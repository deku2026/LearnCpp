// LearnCpp placeholder
// Doc      : part6-branch-e-templates-compile-time-system.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : move_only_function_as_base_cpp23
// Topic id : part6/e/section06/move_only_function_as_base_cpp23
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/move_only_function_as_base_cpp23", run>;

}  // namespace
