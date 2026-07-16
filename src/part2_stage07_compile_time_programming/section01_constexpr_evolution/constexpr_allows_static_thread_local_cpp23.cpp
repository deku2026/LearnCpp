// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_allows_static_thread_local_cpp23
// Topic id : part2/stage07/section01/constexpr_allows_static_thread_local_cpp23
// References: N4950 [dcl.constexpr], [expr.const]; P2242R3, P2647R1.

#include "learn/example_support.hpp"

#include <array>

namespace {

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L

constexpr char hex_digit(unsigned value) noexcept {
    // P2647: a static constexpr local usable in constant expressions may be
    // read while this function itself is being evaluated at compile time.
    static constexpr std::array digits{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    return digits[value & 0xFU];
}

constexpr int thread_local_counter(bool touch_runtime_state) noexcept {
    if (!touch_runtime_state) {
        return 0;
    }

    // P2242 permits this declaration in a constexpr function. Constant
    // evaluation still cannot pass through this thread_local definition.
    thread_local int calls{};
    return ++calls;
}

static_assert(hex_digit(0) == '0');
static_assert(hex_digit(10) == 'a');
static_assert(hex_digit(31) == 'f');
static_assert(thread_local_counter(false) == 0);

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    ::learn::ExampleChecks checks{"part2/stage07/section01/constexpr_allows_static_thread_local_cpp23"};

    LEARN_EXPECT_EQ(checks, hex_digit(0x2A), 'a');
    LEARN_EXPECT_EQ(checks, hex_digit(0xFF), 'f');

    const int first = thread_local_counter(true);
    const int second = thread_local_counter(true);
    LEARN_EXPECT_EQ(checks, second, first + 1);
    LEARN_EXPECT_EQ(checks, thread_local_counter(false), 0);

    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section01/constexpr_allows_static_thread_local_cpp23",
                                               "__cpp_constexpr >= 202211L (P2242/P2647)");
#endif
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section01/constexpr_allows_static_thread_local_cpp23", run>;

}  // namespace
