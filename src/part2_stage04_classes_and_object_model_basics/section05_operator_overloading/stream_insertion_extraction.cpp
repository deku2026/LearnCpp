// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : stream_insertion_extraction
// Topic id : part2/stage04/section05/stream_insertion_extraction
// References: [over.binary], [istream.formatted], [ostream.formatted]

#include "learn/example_support.hpp"

#include <ios>
#include <istream>
#include <ostream>
#include <sstream>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/stream_insertion_extraction";

struct Point {
    int x{};
    int y{};
};

std::ostream& operator<<(std::ostream& output, const Point& point) {
    return output << '(' << point.x << ',' << point.y << ')';
}

std::istream& operator>>(std::istream& input, Point& point) {
    char open{};
    char comma{};
    char close{};
    Point parsed;
    if (input >> open >> parsed.x >> comma >> parsed.y >> close && open == '(' && comma == ',' && close == ')') {
        point = parsed;
    } else {
        input.setstate(std::ios::failbit);
    }
    return input;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Point point;
    std::istringstream valid{"(3,4)"};
    valid >> point;
    LEARN_EXPECT(checks, static_cast<bool>(valid));
    LEARN_EXPECT_EQ(checks, point.x, 3);

    std::ostringstream output;
    output << point;
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"(3,4)"});

    std::istringstream invalid{"3:4"};
    Point unchanged{8, 9};
    invalid >> unchanged;
    LEARN_EXPECT(checks, invalid.fail());
    LEARN_EXPECT_EQ(checks, unchanged.x, 8);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section05/stream_insertion_extraction", run>;

}  // namespace
