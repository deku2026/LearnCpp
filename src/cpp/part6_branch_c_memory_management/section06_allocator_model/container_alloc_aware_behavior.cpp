// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6/C8 分配器感知容器)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : container_alloc_aware_behavior
// Topic id : part6/c/section06/container_alloc_aware_behavior
//
// 要点: 分配器感知容器持有 allocator_type；get_allocator；拷贝/移动语义依赖传播。
// 参考: AllocatorAwareContainer

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

    std::cout << "=== C6 container allocator-aware behavior ===\n";

    std::array<std::byte, 1024> buf{};
    std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};

    std::pmr::vector<int> v{&pool};
    v = {1, 2, 3, 4};
    assert(v.get_allocator().resource() == &pool);

    // 拷贝构造：默认 SOCCC 复制分配器（pmr 复制 allocator 共享同一 resource 指针）
    std::pmr::vector<int> copy = v;
    assert(copy.size() == 4);
    assert(copy.get_allocator().resource() == &pool);

    // 不同 resource 的两个 vector 类型相同，可互操作（赋值可能分配）
    std::pmr::vector<int> other{std::pmr::new_delete_resource()};
    other = v;
    assert(other.size() == 4);
    assert(other.get_allocator().resource() == std::pmr::new_delete_resource());

    std::cout << "  get_allocator(); equal types even with different resources\n";
    std::cout << "container_alloc_aware_behavior: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/container_alloc_aware_behavior", run>;

}  // namespace
