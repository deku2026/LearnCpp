// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_overview
// Topic id : part3/section05/iostream_overview
// References: C++23 [iostreams.base], [iostream.format], [iostream.forward]

#include "learn/example_support.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section05/iostream_overview";

struct Point {
    int x{};
    int y{};

    friend bool operator==(const Point&, const Point&) = default;
};

std::ostream& operator<<(std::ostream& output, const Point& point) {
    return output << '(' << point.x << ',' << point.y << ')';
}

// Parse transactionally: a malformed value does not partly overwrite `point`.
std::istream& operator>>(std::istream& input, Point& point) {
    Point candidate{};
    char left{};
    char comma{};
    char right{};
    if (input >> left >> candidate.x >> comma >> candidate.y >> right && left == '(' && comma == ',' && right == ')') {
        point = candidate;
    } else {
        input.setstate(std::ios_base::failbit);
    }
    return input;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::ostringstream output;
    output << "point=" << Point{3, 4} << ' ' << 21 << '\n';
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"point=(3,4) 21\n"});

    std::istringstream input{"  (8,13)\nAda Lovelace\n"};
    Point point{};
    std::string name;
    input >> point;
    // std::ws consumes the newline left by formatted extraction before getline.
    std::getline(input >> std::ws, name);
    LEARN_EXPECT_EQ(checks, point, (Point{8, 13}));
    LEARN_EXPECT_EQ(checks, name, std::string{"Ada Lovelace"});

    Point unchanged{7, 9};
    std::istringstream malformed{"[1,2]"};
    LEARN_EXPECT(checks, !(malformed >> unchanged));
    LEARN_EXPECT_EQ(checks, unchanged, (Point{7, 9}));

    // The stream conversion checks !fail(), which is the idiom behind
    // `while (input >> value)` loops.
    std::istringstream numbers{"2 3 5 8 end"};
    int sum{};
    int value{};
    while (numbers >> value) {
        sum += value;
    }
    LEARN_EXPECT_EQ(checks, sum, 18);
    LEARN_EXPECT(checks, numbers.fail());
    LEARN_EXPECT(checks, !numbers.bad());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/iostream_overview", run>;

}  // namespace
