// Topic    : pmr 池资源 synchronized / unsynchronized_pool_resource
// Doc      : 第3部分-标准库系统化.md · 库 6.3
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : pool_resource_cpp17
// Topic id : part3/section06/pool_resource_cpp17
// Refs     : https://en.cppreference.com/w/cpp/memory/synchronized_pool_resource
//            https://en.cppreference.com/w/cpp/memory/unsynchronized_pool_resource

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pool_resource_cpp17] unsynchronized_pool_resource ===\n";
    {
        // 单线程池：按大小分桶，适合大量中小对象反复分配
        std::pmr::unsynchronized_pool_resource pool;
        std::pmr::vector<std::pmr::string> v{&pool};
        for (int i = 0; i < 100; ++i) {
            v.emplace_back("item-" + std::to_string(i));
        }
        assert(v.size() == 100);
        assert(v.front().starts_with("item-"));
        std::cout << "unsync pool strings=" << v.size() << " last=" << v.back() << '\n';
        // 清空后内存通常仍留在池中，供下次复用
        v.clear();
        v.emplace_back("reused-bucket");
        assert(v[0] == "reused-bucket");
        std::cout << "after clear+push: " << v[0] << '\n';
    }

    std::cout << "=== synchronized_pool_resource（线程安全池） ===\n";
    {
        std::pmr::synchronized_pool_resource pool;
        std::pmr::vector<int> a{&pool};
        std::pmr::vector<int> b{&pool};
        for (int i = 0; i < 50; ++i) {
            a.push_back(i);
            b.push_back(i * 2);
        }
        assert(a.size() == 50 && b.size() == 50);
        assert(b[10] == 20);
        std::cout << "sync pool two vectors sizes=" << a.size() << ',' << b.size() << '\n';
        std::cout << "use synchronized when multiple threads share one resource\n";
    }

    std::cout << "=== options 探测（最大块等） ===\n";
    {
        std::pmr::pool_options opt;
        opt.max_blocks_per_chunk = 16;
        opt.largest_required_pool_block = 256;
        std::pmr::unsynchronized_pool_resource pool{opt};
        std::pmr::vector<char> buf{&pool};
        buf.resize(128);
        assert(buf.size() == 128);
        auto got = pool.options();
        std::cout << "options largest_required_pool_block=" << got.largest_required_pool_block
                  << " max_blocks_per_chunk=" << got.max_blocks_per_chunk << '\n';
    }

    std::cout << "monotonic vs pool: monotonic = build-and-throw-away; "
                 "pool = reuse similar sizes\n";
    std::cout << "[pool_resource_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/pool_resource_cpp17", run>;

}  // namespace
