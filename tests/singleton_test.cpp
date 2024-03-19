#include <gtest/gtest.h>
#include <thread>

#include "dns_cache.h"


TEST(DnscTests, Test_SingletonBase) {
    DNSC::DNSCache& cache = DNSC::DNSCache::get_instance(26);

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        cache.update(s, s);
    }

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        ASSERT_EQ(cache.resolve(s), s);
    }
}


TEST(DnscTests, Test_SingletonMultiUpdate) {
    DNSC::DNSCache& cache = DNSC::DNSCache::get_instance(26);

    int threads_count = 10;
    std::vector<std::thread> ts;
    auto task = []() {
        DNSC::DNSCache& cache = DNSC::DNSCache::get_instance(26);

        for (int k = 0; k < 1000; k++)
            for (char c = 'a'; c <= 'z'; c++) {
                std::string s = std::string{c} + "1";
                cache.update(s, s);
            }
    };

    for (int i = 0; i < threads_count; i++)
        ts.emplace_back(task);

    for (int i = 0; i < threads_count; i++)
        ts[i].join();    

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s = std::string{c} + "1";
        ASSERT_EQ(cache.resolve(s), s);
    }
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}