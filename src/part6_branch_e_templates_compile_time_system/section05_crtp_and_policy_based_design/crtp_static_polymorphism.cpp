// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : crtp_static_polymorphism
// Topic id : part6/e/section05/crtp_static_polymorphism

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <numbers>

namespace {

template <class Derived>
class ShapeBase {
public:
    [[nodiscard]] constexpr double area() const noexcept { return static_cast<const Derived&>(*this).area_impl(); }
    [[nodiscard]] constexpr int dimension_count() const noexcept { return Derived::dimensions; }
};

class Rectangle final : public ShapeBase<Rectangle> {
public:
    constexpr Rectangle(double width, double height) noexcept : width_(width), height_(height) {}
    [[nodiscard]] constexpr double area_impl() const noexcept { return width_ * height_; }
    static constexpr int dimensions = 2;

private:
    double width_;
    double height_;
};

class Circle final : public ShapeBase<Circle> {
public:
    explicit constexpr Circle(double radius) noexcept : radius_(radius) {}
    [[nodiscard]] constexpr double area_impl() const noexcept { return radius_ * radius_ * std::numbers::pi; }
    static constexpr int dimensions = 2;

private:
    double radius_;
};

template <class Shape>
[[nodiscard]] constexpr double total_area(const Shape& first, const Shape& second) noexcept {
    return first.area() + second.area();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section05/crtp_static_polymorphism"};

    constexpr Rectangle rectangle{6.0, 7.0};
    static_assert(rectangle.area() == 42.0);
    static_assert(rectangle.dimension_count() == 2);
    LEARN_EXPECT_EQ(checks, total_area(Rectangle{2.0, 3.0}, Rectangle{4.0, 5.0}), 26.0);
    LEARN_EXPECT(checks, Circle{1.0}.area() > 3.14);
    LEARN_EXPECT_EQ(checks, Circle{1.0}.dimension_count(), 2);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/crtp_static_polymorphism", run>;

}  // namespace
