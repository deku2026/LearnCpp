// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : public_inheritance
// Topic id : part2/stage04/section08/public_inheritance
// References: [class.derived], C++ Core Guidelines C.35

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section08/public_inheritance";

class Rectangle {
public:
    Rectangle(int width, int height) : width_(width), height_(height) {}
    [[nodiscard]] int area() const noexcept { return width_ * height_; }

private:
    int width_;
    int height_;
};

class ColoredRectangle : public Rectangle {
public:
    ColoredRectangle(int width, int height, int color) : Rectangle(width, height), color_(color) {}
    [[nodiscard]] int color() const noexcept { return color_; }

private:
    int color_;
};

static_assert(std::is_convertible_v<ColoredRectangle*, Rectangle*>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const ColoredRectangle colored{3, 4, 0xFF00};
    const Rectangle& rectangle = colored;
    LEARN_EXPECT_EQ(checks, rectangle.area(), 12);
    LEARN_EXPECT_EQ(checks, colored.color(), 0xFF00);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section08/public_inheritance", run>;

}  // namespace
