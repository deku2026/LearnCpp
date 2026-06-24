// LearnCpp placeholder
// Doc      : part6-branch-d-name-lookup-overload-resolution.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_for_cout_operator_and_swap
// Topic id : part6/d/section01/adl_for_cout_operator_and_swap
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/adl_for_cout_operator_and_swap", run>;

}  // namespace
