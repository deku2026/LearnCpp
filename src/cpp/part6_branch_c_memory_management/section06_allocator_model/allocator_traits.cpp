// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 allocator_traits)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_traits
// Topic id : part6/c/section06/allocator_traits
//
// 要点: 容器经 allocator_traits 间接调用；traits 提供默认 construct/destroy。
// 参考: https://en.cppreference.com/w/cpp/memory/allocator_traits

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 allocator_traits ===\n";

    using A = std::allocator<std::string>;
    using AT = std::allocator_traits<A>;
    A alloc;
    std::string* p = AT::allocate(alloc, 1);
    AT::construct(alloc, p, "hello");
    assert(*p == "hello");
    AT::destroy(alloc, p);
    AT::deallocate(alloc, p, 1);

    // std::allocator 实际 traits：
    // - POCCA / POCS: false（默认不传播）
    // - POCMA: true（std::allocator 自 C++11 起提供 true_type，便于 move 时带走分配器）
    static_assert(!AT::propagate_on_container_copy_assignment::value);
    static_assert(AT::propagate_on_container_move_assignment::value);
    static_assert(!AT::propagate_on_container_swap::value);

    std::vector<int> v{1, 2, 3};  // 内部用 traits
    assert(v.size() == 3);

    std::cout << "  containers call traits::allocate/construct/...\n";
    std::cout << "allocator_traits: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_traits", run>;

}  // namespace
