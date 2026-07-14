// LearnCpp placeholder
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : standard_layout_versus_aggregate_versus_pod
// Topic id : part6/a/section01/standard_layout_versus_aggregate_versus_pod
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/standard_layout_versus_aggregate_versus_pod", run>;

}  // namespace
