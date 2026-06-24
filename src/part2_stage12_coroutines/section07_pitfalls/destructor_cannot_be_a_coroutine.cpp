// LearnCpp placeholder
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : destructor_cannot_be_a_coroutine
// Topic id : part2/stage12/section07/destructor_cannot_be_a_coroutine
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

LEARN_TOPIC("part2/stage12/section07/destructor_cannot_be_a_coroutine", run);

}  // namespace
