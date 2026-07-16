// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : pimpl_idiom
// Topic id : part2/stage15/section01/pimpl_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section01/pimpl_idiom";

class Widget {
public:
    Widget(std::string name, int value);
    Widget(const Widget& other);
    Widget(Widget&&) noexcept = default;
    Widget& operator=(Widget replacement) noexcept;
    ~Widget();

    friend void swap(Widget& left, Widget& right) noexcept { left.impl_.swap(right.impl_); }
    std::string label() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

struct Widget::Impl {
    std::string name;
    int value;
};

Widget::Widget(std::string name, int value) : impl_(std::make_unique<Impl>(std::move(name), value)) {}
Widget::Widget(const Widget& other) : impl_(std::make_unique<Impl>(*other.impl_)) {}
Widget& Widget::operator=(Widget replacement) noexcept {
    swap(*this, replacement);
    return *this;
}
Widget::~Widget() = default;  // Defined where Impl is complete.
std::string Widget::label() const {
    return impl_->name + ':' + std::to_string(impl_->value);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Widget original{"sensor", 7};
    Widget copy = original;
    LEARN_EXPECT_EQ(checks, original.label(), std::string{"sensor:7"});
    LEARN_EXPECT_EQ(checks, copy.label(), original.label());
    original = Widget{"updated", 9};
    LEARN_EXPECT_EQ(checks, copy.label(), std::string{"sensor:7"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section01/pimpl_idiom", run>;

}  // namespace
