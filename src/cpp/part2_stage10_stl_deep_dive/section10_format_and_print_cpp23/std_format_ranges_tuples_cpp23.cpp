// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_format_ranges_tuples_cpp23
//
// Covers: format ranges and tuples C++23 P2286

#include "learn/topic_registry.hpp"

#include <string>
#include <tuple>
#include <vector>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L && __has_include(<format>)
    std::vector<int> v{1, 2, 3};
    const auto s = std::format("{}", v);
    LEARN_CHECK(s.find('1') != std::string::npos);
    LEARN_CHECK(s.find('3') != std::string::npos);
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    std::vector<int> v{1, 2, 3};
    const auto s = std::format("{} {} {}", v[0], v[1], v[2]);
    LEARN_CHECK(s == "1 2 3");
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L && __has_include(<format>)
    auto t = std::make_tuple(1, "x");
    const auto s = std::format("{}", t);
    LEARN_CHECK(s.find('1') != std::string::npos);
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    const auto s = std::format("{} {}", 1, "x");
    LEARN_CHECK(s == "1 x");
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L && __has_include(<format>)
    std::vector<std::pair<int, int>> v{{1, 2}, {3, 4}};
    const auto s = std::format("{}", v);
    LEARN_CHECK(!s.empty());
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    LEARN_CHECK(std::format("{}", 42) == "42");
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/std_format_ranges_tuples_cpp23", run>;

}  // namespace
