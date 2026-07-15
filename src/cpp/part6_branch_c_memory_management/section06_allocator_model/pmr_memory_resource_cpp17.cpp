// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C8 pmr memory_resource)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pmr_memory_resource_cpp17
// Topic id : part6/c/section06/pmr_memory_resource_cpp17
//
// 要点: memory_resource 运行期多态；polymorphic_allocator 接资源；类型不随资源变。
// 参考: https://en.cppreference.com/w/cpp/memory/memory_resource

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <memory_resource>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C8 pmr::memory_resource ===\n";

    std::pmr::memory_resource* nd = std::pmr::new_delete_resource();
    std::pmr::memory_resource* null_r = std::pmr::null_memory_resource();
    assert(nd != nullptr);
    assert(null_r != nullptr);

    std::pmr::vector<int> a{nd};
    a.push_back(1);
    a.push_back(2);
    assert(a.size() == 2);

    // 不同 resource，同一容器类型
    std::array<std::byte, 512> buf{};
    std::pmr::monotonic_buffer_resource mono{buf.data(), buf.size(), nd};
    std::pmr::vector<int> b{&mono};
    b.push_back(3);
    assert(b.front() == 3);

    // null resource：分配即失败
    bool threw = false;
    try {
        std::pmr::vector<int> c{null_r};
        c.reserve(1);
    } catch (const std::bad_alloc&) {
        threw = true;
    }
    assert(threw);

    std::cout << "pmr_memory_resource_cpp17: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pmr_memory_resource_cpp17", run>;

}  // namespace
