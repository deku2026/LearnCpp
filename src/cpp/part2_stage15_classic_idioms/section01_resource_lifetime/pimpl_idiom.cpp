// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : pimpl_idiom
// Topic id : part2/stage15/section01/pimpl_idiom
//
// Covers: pimpl compilation firewall

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>

namespace {

class Widget {
public:
    Widget();
    ~Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;
    int value() const;
    void set(int v);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

struct Widget::Impl {
    int v = 0;
    std::string tag = "w";
};

Widget::Widget() : impl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;
int Widget::value() const {
    return impl_->v;
}
void Widget::set(int v) {
    impl_->v = v;
}

void demo_basics() {
    Widget w;
    assert(w.value() == 0);
    w.set(3);
    assert(w.value() == 3);
}

void demo_intermediate() {
    Widget w;
    w.set(10);
    Widget w2 = std::move(w);
    assert(w2.value() == 10);
}

void demo_expert() {
    // Header only needs unique_ptr incomplete type; Impl stays in .cpp in real code.
    Widget a;
    Widget b;
    a.set(1);
    b.set(2);
    assert(a.value() + b.value() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/pimpl_idiom", run>;

}  // namespace
