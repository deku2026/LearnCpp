// LearnCpp placeholder
// Doc      : part6-branch-f-memory-model-and-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : function_local_static_thread_safety
// Topic id : part6/f/section05/function_local_static_thread_safety
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

LEARN_TOPIC("part6/f/section05/function_local_static_thread_safety", run);

}  // namespace
