// Topic    : pmr::memory_resource + polymorphic_allocator（C++17）
// Doc      : 第3部分-标准库系统化.md · 库 6.3
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : pmr_memory_resource_cpp17
// Topic id : part3/section06/pmr_memory_resource_cpp17
// Refs     : https://en.cppreference.com/w/cpp/memory/memory_resource
//            https://en.cppreference.com/w/cpp/memory/polymorphic_allocator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory_resource>
#include <new>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// 计数包装：观察分配次数
struct counting_resource : std::pmr::memory_resource {
    std::pmr::memory_resource* upstream;
    std::size_t allocs{0};
    std::size_t bytes{0};

    explicit counting_resource(std::pmr::memory_resource* up = std::pmr::new_delete_resource()) : upstream{up} {}

protected:
    void* do_allocate(std::size_t bytes, std::size_t align) override {
        ++allocs;
        this->bytes += bytes;
        return upstream->allocate(bytes, align);
    }
    void do_deallocate(void* p, std::size_t bytes, std::size_t align) override {
        upstream->deallocate(p, bytes, align);
    }
    bool do_is_equal(const memory_resource& other) const noexcept override { return this == &other; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pmr_memory_resource_cpp17] 默认 new_delete_resource ===\n";
    {
        counting_resource counter;
        std::pmr::vector<int> v{&counter};
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        assert(v.size() == 3);
        std::cout << "vector allocs=" << counter.allocs << " bytes~=" << counter.bytes << '\n';
        assert(counter.allocs >= 1);
    }

    std::cout << "=== polymorphic_allocator：类型相同、策略不同 ===\n";
    {
        counting_resource a;
        counting_resource b;
        std::pmr::vector<int> va{&a};
        std::pmr::vector<int> vb{&b};
        // 类型相同，可放入同一容器/接口
        static_assert(std::is_same_v<decltype(va), decltype(vb)>);
        va.push_back(10);
        vb.push_back(20);
        std::cout << "same type pmr::vector, different resources: "
                  << "a.allocs=" << a.allocs << " b.allocs=" << b.allocs << '\n';
    }

    std::cout << "=== null_memory_resource：分配即 bad_alloc ===\n";
    {
        std::pmr::vector<int> v{std::pmr::null_memory_resource()};
        bool threw = false;
        try {
            v.push_back(1);
        } catch (const std::bad_alloc&) {
            threw = true;
        }
        assert(threw);
        std::cout << "null_memory_resource threw bad_alloc as expected\n";
    }

    std::cout << "=== get_default_resource 探测 ===\n";
    {
        auto* def = std::pmr::get_default_resource();
        assert(def != nullptr);
        std::cout << "default resource == new_delete? " << (def == std::pmr::new_delete_resource()) << '\n';
    }

    std::cout << "[pmr_memory_resource_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/pmr_memory_resource_cpp17", run>;

}  // namespace
