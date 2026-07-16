// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : if_consteval_branch_cpp23
// Topic id : part2/stage15/section03/if_consteval_branch_cpp23
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section03/if_consteval_branch_cpp23";

constexpr int normalized(int value) {
    if consteval {
        return value * 10;
    } else {
        return value < 0 ? 0 : value;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr int compiled = normalized(3);
    int runtime_input = -2;
    static_assert(compiled == 30);
    LEARN_EXPECT_EQ(checks, normalized(runtime_input), 0);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section03/if_consteval_branch_cpp23", run>;

}  // namespace
