// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : stack_versus_heap
// Topic id : part6/c/section01/stack_versus_heap
//
// Covers: automatic stack vs dynamic heap allocation tradeoffs

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <memory>

namespace {

void demo_basics() {
    int stack = 42;
    LEARN_CHECK(stack == 42);
    int* heap = new int(42);
    LEARN_CHECK(*heap == 42);
    delete heap;
}

void demo_intermediate() {
    auto up = std::make_unique<int>(7);
    LEARN_CHECK(*up == 7);
    int arr[4] = {1, 2, 3, 4};
    LEARN_CHECK(arr[3] == 4);
}

void demo_expert() {
    // Stack: fast, scoped, size limited. Heap: flexible size, manual/RAII lifetime.
    constexpr int n = 8;
    auto buf = std::make_unique<int[]>(n);
    for (int i = 0; i < n; ++i) buf[static_cast<std::size_t>(i)] = i;
    LEARN_CHECK(buf[7] == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/stack_versus_heap", run>;

}  // namespace
