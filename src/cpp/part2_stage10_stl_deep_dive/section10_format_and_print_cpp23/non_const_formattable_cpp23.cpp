// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : non_const_formattable_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/non_const_formattable_cpp23
//
// Covers: formatter / formattable for user types

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif

namespace {

struct Point {
    int x;
    int y;
};

}  // namespace

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
template <>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({},{})", p.x, p.y);
    }
};
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    Point p{1, 2};
    LEARN_CHECK(std::format("{}", p) == "(1,2)");
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    Point p{3, 4};
    LEARN_CHECK(std::format("pt={}", p) == "pt=(3,4)");
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // C++23 formattable concept / const-correct formatters
    Point p{0, 0};
    const Point& cp = p;
    LEARN_CHECK(std::format("{}", cp) == "(0,0)");
#else
    LEARN_CHECK(true);
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
    ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/non_const_formattable_cpp23", run>;

}  // namespace
