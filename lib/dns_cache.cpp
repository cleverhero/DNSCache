#include "dns_cache.h"


namespace DNSC::Detail{

//
// Complexity: O(1) average
// Exceptions: Strong exception safety guarantee
//
void DNSCacheImpl::update(const std::string& name, const std::string& ip) {
    std::lock_guard<std::mutex> guard(mutex_);

    auto [iter, is_created] = insert(name, ip);

    if (!is_created) {
        iter->second->ip = ip;
        to_front_by_map_iterator(iter);
    }
}


//
// Complexity: O(1) average
// Exceptions: Does not throw
//
std::string DNSCacheImpl::resolve(const std::string& name) noexcept {
    std::lock_guard<std::mutex> guard(mutex_);

    auto iter = cache_map.find(name);
    if (iter == cache_map.end())
        return {};

    to_front_by_map_iterator(iter);
    return iter->second->ip;
}


//
// Complexity: O(1) average
// Exceptions: Does not throw
//
void DNSCacheImpl::evict() noexcept {
    auto& last = cache_list.back();
    cache_map.erase(last.name);
    cache_list.pop_back();
}


//
// Complexity: O(1) average
// Exceptions: Strong exception safety guarantee
//
std::pair<DNSCacheImpl::map_iterator, bool>
DNSCacheImpl::insert(const std::string& name, const std::string& ip) {
    auto iter = cache_map.find(name);
    if (iter != cache_map.end())
        return {iter, false};

    cache_list.emplace_front(name, ip);
    std::pair<DNSCacheImpl::map_iterator, bool> res;
    try {
        res = cache_map.insert({name, cache_list.begin()});
    }
    catch (...) {
        cache_list.pop_front();
        throw;
    }

    if (cache_list.size() > max_size)
        evict();

    return res;
}


//
// Complexity: O(1)
// Exceptions: Does not throw
//
void DNSCacheImpl::to_front_by_map_iterator(map_iterator iter) noexcept {
    cache_list.splice(cache_list.begin(), cache_list, iter->second);
}

}