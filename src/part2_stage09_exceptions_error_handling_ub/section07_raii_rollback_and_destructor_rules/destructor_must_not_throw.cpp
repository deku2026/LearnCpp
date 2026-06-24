// LearnCpp placeholder
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_must_not_throw
// Topic id : part2/stage09/section07/destructor_must_not_throw
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

LEARN_TOPIC("part2/stage09/section07/destructor_must_not_throw", run);

}  // namespace
