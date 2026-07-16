// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_unreachable_cpp23_recap
// Topic id : part2/stage10/section05/std_unreachable_cpp23_recap
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string_view>
#include <utility>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_unreachable_cpp23_recap";

enum class State { idle, busy };

constexpr int code(State state) {
    switch (state) {
        case State::idle:
            return 0;
        case State::busy:
            return 1;
    }
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    std::unreachable();
#else
    return -1;
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    static_assert(code(State::idle) == 0);
    LEARN_EXPECT_EQ(checks, code(State::busy), 1);
    // Reaching unreachable is undefined; it must follow an invariant, never replace validation.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_unreachable_cpp23_recap", run>;

}  // namespace
