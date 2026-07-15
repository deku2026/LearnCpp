// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_format_ranges_tuples_cpp23
//
// Covers: format ranges and tuples C++23 P2286

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <tuple>
#include <vector>
#include <version>

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    std::vector<int> v{1, 2, 3};
    const auto s = std::format("{}", v);
    assert(s.find('1') != std::string::npos);
    assert(s.find('3') != std::string::npos);
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    std::vector<int> v{1, 2, 3};
    const auto s = std::format("{} {} {}", v[0], v[1], v[2]);
    assert(s == "1 2 3");
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    auto t = std::make_tuple(1, "x");
    const auto s = std::format("{}", t);
    assert(s.find('1') != std::string::npos);
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const auto s = std::format("{} {}", 1, "x");
    assert(s == "1 x");
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    std::vector<std::pair<int, int>> v{{1, 2}, {3, 4}};
    const auto s = std::format("{}", v);
    assert(!s.empty());
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    assert(std::format("{}", 42) == "42");
#else
    assert(true);
#endif
}

}  // namespace

namespace {

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
