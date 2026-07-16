// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_unreachable_cpp23
// Topic id : part2/stage09/section06/std_unreachable_cpp23
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>
#include <utility>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section06/std_unreachable_cpp23";

enum class Direction { left, right };

constexpr int sign(Direction direction) {
    switch (direction) {
        case Direction::left:
            return -1;
        case Direction::right:
            return 1;
    }
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    std::unreachable();  // This is a contract with the optimizer, never a checked failure path.
#else
    return 0;
#endif
}

static_assert(sign(Direction::left) == -1);
static_assert(sign(Direction::right) == 1);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, sign(Direction::left), -1);
    LEARN_EXPECT_EQ(checks, sign(Direction::right), 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section06/std_unreachable_cpp23", run>;

}  // namespace
