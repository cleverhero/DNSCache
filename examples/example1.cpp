#include <iostream>

#include "dns_cache.h"


int main() {
    DNSC::DNSCache& cache = DNSC::DNSCache::get_instance(20);

    for (char i = 'a'; i <= 'z'; i++) {
        std::string s{i};
        cache.update(s, s);
    }

    for (char i = 'a'; i <= 'z'; i++) {
        std::string s{i};
        std::cout << cache.resolve(s) << std::endl;
    }

    for (char i = 'a'; i <= 'z'; i++) {
        std::string s{i};
        std::string s2{(char)('z' - i + 'a')};
        cache.update(s2, s);
    }

    for (char i = 'a'; i <= 'z'; i++) {
        std::string s{i};
        std::cout << cache.resolve(s) << std::endl;
    }
    
    return 0;
}