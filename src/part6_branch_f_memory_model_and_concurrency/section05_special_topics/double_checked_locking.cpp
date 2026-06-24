// LearnCpp placeholder
// Doc      : part6-branch-f-memory-model-and-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : double_checked_locking
// Topic id : part6/f/section05/double_checked_locking
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/double_checked_locking", run>;

}  // namespace
