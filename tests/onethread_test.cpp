#include <gtest/gtest.h>
#include <vector>

#include "dns_cache.h"


TEST(DnscTests, Test_Base) {
    DNSC::Detail::DNSCacheImpl cache{26};

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        cache.update(s, s);
    }

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        ASSERT_EQ(cache.resolve(s), s);
    }
}


TEST(DnscTests, Test_Evict) {
    const size_t cache_size = 20;
    DNSC::Detail::DNSCacheImpl cache{cache_size};

    for (char c = 'z'; c >= 'a'; c--) {
        std::string s{c};
        cache.update(s, s);
    }

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        if (c < 'a' + cache_size)
            ASSERT_EQ(cache.resolve(s), s);
        else
            ASSERT_EQ(cache.resolve(s), std::string{});
    }
}

TEST(DnscTests, Test_Evict2) {
    DNSC::Detail::DNSCacheImpl cache{5};
    std::vector<std::string> v{
        "a", "b", "c", "d", "e", "f", "g"
    };

    for (int i = 0; i < 5; i++)
        cache.update(v[i], v[i]); // e d c b a

    cache.update(v[5], v[5]);     // f e d c b
    ASSERT_EQ(cache.resolve(v[0]), std::string{});

    cache.update(v[1], v[1]);     // b f e d c
    cache.update(v[6], v[6]);     // g b f e d
    ASSERT_EQ(cache.resolve(v[2]), std::string{});
}

TEST(DnscTests, Test_Update) {
    DNSC::Detail::DNSCacheImpl cache{26};

    for (char c = 'a'; c <= 'z'; c++) {
        std::string s{c};
        cache.update(s, s);
    }

    for (char c = 'a'; c <= 'z'; c++) {
        std::string name{c};
        std::string ip{static_cast<char>(c + 1)};
        cache.update(name, ip);
    }

    for (char c = 'a'; c <= 'z'; c++) {
        std::string name{c};
        std::string ip{static_cast<char>(c + 1)};
        ASSERT_EQ(cache.resolve(name), ip);
    }
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}