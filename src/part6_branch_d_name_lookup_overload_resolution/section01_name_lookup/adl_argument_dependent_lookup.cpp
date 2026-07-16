// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_argument_dependent_lookup
// Topic id : part6/d/section01/adl_argument_dependent_lookup
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <cmath>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/adl_argument_dependent_lookup";

namespace geometry {

struct Point {
    double x{};
    double y{};
};

[[nodiscard]] double magnitude(Point point) {
    return std::hypot(point.x, point.y);
}

}  // namespace geometry

template <class T>
[[nodiscard]] auto measure(const T& value) {
    // magnitude is dependent on T. At instantiation ADL inspects the
    // associated namespace of T and finds geometry::magnitude.
    return magnitude(value);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const geometry::Point point{3.0, 4.0};
    LEARN_EXPECT_EQ(checks, magnitude(point), 5.0);  // Found solely by ADL.
    LEARN_EXPECT_EQ(checks, measure(point), 5.0);
    LEARN_EXPECT_EQ(checks, geometry::magnitude(point), 5.0);  // Explicit alternative.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/adl_argument_dependent_lookup", run>;

}  // namespace
