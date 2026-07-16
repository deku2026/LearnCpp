// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_std_string_cpp20
// Topic id : part2/stage07/section05/constexpr_std_string_cpp20
// References: N4950 [basic.string], [expr.const]; P0980R1, P0784R7.

#include "learn/example_support.hpp"

#include <string>

namespace {

#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L

constexpr bool string_edit_pipeline() {
    std::string text{"  cpp23  "};
    text.erase(0, 2);
    text.erase(text.size() - 2);
    text.replace(0, 3, "C++");
    text += " constexpr";
    return text == "C++23 constexpr" && text.find("constexpr") == 6;
}

constexpr std::size_t generated_length() {
    std::string text;
    text.append(32, 'x');  // Allocation is transient within this evaluation.
    text.append(10, 'y');
    text.erase(0, 2);
    return text.size();
}

#if !defined(LEARNCPP_HAS_CONSTEXPR_STRING_EVALUATION) || LEARNCPP_HAS_CONSTEXPR_STRING_EVALUATION
static_assert(string_edit_pipeline());
static_assert(generated_length() == 40);
#endif

#if 0
// Storage allocated during constant evaluation cannot escape into the runtime program.
constexpr std::string persistent_allocation(128, 'x');
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
    ::learn::ExampleChecks checks{"part2/stage07/section05/constexpr_std_string_cpp20"};

    LEARN_EXPECT(checks, string_edit_pipeline());
    LEARN_EXPECT_EQ(checks, generated_length(), 40uz);

    std::string runtime_text{"compile"};
    runtime_text += " time";
    LEARN_EXPECT_EQ(checks, runtime_text, std::string{"compile time"});
#if defined(LEARNCPP_HAS_CONSTEXPR_STRING_EVALUATION) && !LEARNCPP_HAS_CONSTEXPR_STRING_EVALUATION
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return ::learn::ExampleChecks::unavailable("part2/stage07/section05/constexpr_std_string_cpp20",
                                               "constant evaluation of dynamically allocated std::string");
#else
    return checks.result();
#endif
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section05/constexpr_std_string_cpp20",
                                               "__cpp_lib_constexpr_string >= 201907L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_std_string_cpp20", run>;

}  // namespace
