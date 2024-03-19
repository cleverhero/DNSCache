#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "dns_cache.h"


TEST(DnscTests, Test_MultiUpdate) {
    DNSC::Detail::DNSCacheImpl cache{26};

    int threads_count = 10;
    std::vector<std::thread> ts;
    auto task = [&cache]() {
        for (int k = 0; k < 1000; k++)
            for (char c = 'a'; c <= 'z'; c++) {
                std::string s{c};
                cache.update(s, s);
            }
    };

    for (int i = 0; i < threads_count; i++)
        ts.emplace_back(task);

    for (int i = 0; i < threads_count; i++)
        ts[i].join();    

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        ASSERT_EQ(cache.resolve(s), s);
    }
}


TEST(DnscTests, Test_MultiResolve) {
    DNSC::Detail::DNSCacheImpl cache{26};

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        cache.update(s, s);
    }

    int threads_count = 10;
    std::vector<std::thread> ts;
    auto task = [&cache]() {
        for (int k = 0; k < 1000; k++) {
            for (char c = 'a'; c <= 'z'; c++) {
                std::string s{c};
                ASSERT_EQ(cache.resolve(s), s);
            }
        }
    };

    for (int i = 0; i < threads_count; i++)
        ts.emplace_back(task);

    for (int i = 0; i < threads_count; i++)
        ts[i].join();
}


TEST(DnscTests, Test_MultiUpdateResolve) {
    DNSC::Detail::DNSCacheImpl cache{26};

    int threads_count = 10;
    std::vector<std::thread> ts;
    auto task = [&cache]() {
        for (int k = 0; k < 1000; k++) {
            for (char c = 'a'; c <= 'z'; c++) {
                std::string name{c};
                std::string ip = name + std::to_string(k);
                cache.update(name, ip);
            }

            for (char c = 'a'; c <= 'z'; c++) {
                std::string name{c};
                ASSERT_NE(cache.resolve(name), std::string{});
            }
        }
    };

    for (int i = 0; i < threads_count; i++)
        ts.emplace_back(task);

    for (int i = 0; i < threads_count; i++)
        ts[i].join();
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}