#include <cstddef>
#include <cassert>
#include <string>
#include <unordered_map>
#include <list>
#include <mutex>


namespace DNSC {
    namespace Detail {
        class DNSCacheImpl final {
        public:
            explicit DNSCacheImpl(size_t max_size_): max_size(max_size_) {
                assert(max_size_ > 0);
            }
            void update(const std::string& name, const std::string& ip);
            std::string resolve(const std::string& name) noexcept;

        private:
            struct Node {
                std::string name;
                std::string ip;

                Node(const std::string& name_, const std::string& ip_): name(name_), ip(ip_) {}
            };
            using list_iterator = std::list<Node>::iterator;
            using map_iterator = std::unordered_map<std::string, list_iterator>::iterator;

            void evict() noexcept;
            void to_front_by_map_iterator(map_iterator iter) noexcept;
            std::pair<map_iterator, bool> insert(const std::string& name, const std::string& ip);

            size_t max_size;
            std::list<Node> cache_list;
            std::unordered_map<std::string, list_iterator> cache_map;
            std::mutex mutex_;
        };
    }

    class DNSCache final {
    public:
        static DNSCache& get_instance(size_t max_size) {
            static DNSCache cache{max_size};
            return cache;
        }

        void update(const std::string& name, const std::string& ip) {
            impl.update(name, ip);
        }

        std::string resolve(const std::string& name) {
            return impl.resolve(name);
        }

        DNSCache(const DNSCache&) = delete;
        void operator=(const DNSCache&) = delete;

    private:
        explicit DNSCache(size_t max_size): impl(max_size) {}
        Detail::DNSCacheImpl impl;
    };
}
