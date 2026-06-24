// LearnCpp placeholder
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : forbidden_user_specialization_of_allocator_traits_cpp23
// Topic id : part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23
//
// TODO: read cppreference, sketch a minimal example, check godbolt / C++ Insights,
//       then replace this empty run() body with real demo code.

#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23", run>;

}  // namespace
