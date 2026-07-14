// LearnCpp placeholder
// Doc      : part6-branch-f-memory-model-and-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : memory_barriers
// Topic id : part6/f/section04/memory_barriers
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/memory_barriers", run>;

}  // namespace
