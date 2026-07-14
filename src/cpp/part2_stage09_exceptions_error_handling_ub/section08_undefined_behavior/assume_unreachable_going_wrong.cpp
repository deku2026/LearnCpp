// LearnCpp placeholder
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : assume_unreachable_going_wrong
// Topic id : part2/stage09/section08/assume_unreachable_going_wrong
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/assume_unreachable_going_wrong", run>;

}  // namespace
