// LearnCpp placeholder
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : static_assert
// Topic id : part2/stage07/section04/static_assert
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/static_assert", run>;

}  // namespace
