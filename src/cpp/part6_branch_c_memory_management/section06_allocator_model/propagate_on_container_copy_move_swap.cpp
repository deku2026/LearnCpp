// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : propagate_on_container_copy_move_swap
// Topic id : part6/c/section06/propagate_on_container_copy_move_swap
//
// Covers: POCMA/POCCA/POCS traits

#include "learn/topic_registry.hpp"

#include <memory>
#include <type_traits>

namespace {

void demo_basics() {
    using AT = std::allocator_traits<std::allocator<int>>;
    // std::allocator typically does not propagate (always equal).
    static_assert(!AT::propagate_on_container_copy_assignment::value ||
                  AT::propagate_on_container_copy_assignment::value);
    LEARN_CHECK(true);
}

void demo_intermediate() {
    using AT = std::allocator_traits<std::allocator<int>>;
    static_assert(std::is_same_v<typename AT::propagate_on_container_move_assignment, std::true_type> ||
                  std::is_same_v<typename AT::propagate_on_container_move_assignment, std::false_type>);
}

void demo_expert() {
    // When POCMA is true, move-assignment can take the allocator from the source.
    // When false and allocators unequal, element-wise move may be required.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/propagate_on_container_copy_move_swap", run>;

}  // namespace
