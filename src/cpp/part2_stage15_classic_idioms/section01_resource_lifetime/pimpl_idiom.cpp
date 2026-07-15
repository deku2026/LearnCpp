// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §1.5
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : pimpl_idiom
// Topic id : part2/stage15/section01/pimpl_idiom
//
// Pimpl (pointer to implementation): hide private details behind an opaque pointer.
// Motive : isolate compile deps, stabilize ABI, keep headers lean.
// Modern : std::unique_ptr<Impl> (Qt d-pointer is the same idea).
// Pitfall: ~Widget / move ops must be defined where Impl is complete (usually .cpp).
//          Single-TU demo below defines them after Impl is complete.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

// ---------- "widget.h" surface (what clients see) ----------
class Widget {
public:
    Widget();
    explicit Widget(std::string name);
    ~Widget();  // out-of-line: Impl incomplete at point of declaration

    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    void do_something();
    void add_tag(std::string tag);
    const std::string& name() const;
    std::size_t tag_count() const;

private:
    class Impl;  // incomplete here
    std::unique_ptr<Impl> pimpl_;
};

// ---------- "widget.cpp" body ----------
class Widget::Impl {
public:
    std::string name;
    std::vector<std::string> tags;  // heavy header only needed in the impl TU
    int counter = 0;

    explicit Impl(std::string n) : name(std::move(n)) {}

    void do_something() {
        ++counter;
        std::cout << "  Widget::Impl::do_something name=" << name << " counter=" << counter << " tags=" << tags.size()
                  << '\n';
    }
};

Widget::Widget() : Widget(std::string{"default"}) {}
Widget::Widget(std::string name) : pimpl_(std::make_unique<Impl>(std::move(name))) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::do_something() {
    pimpl_->do_something();
}
void Widget::add_tag(std::string tag) {
    pimpl_->tags.push_back(std::move(tag));
}
const std::string& Widget::name() const {
    return pimpl_->name;
}
std::size_t Widget::tag_count() const {
    return pimpl_->tags.size();
}

// ---------- contrast: all-inline class leaks every private header ----------
struct FatInlineWidget {
    std::string name;
    std::vector<std::string> tags;
    // any change recompiles all clients that include this definition
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== unique_ptr pimpl ==\n";
    Widget w("panel");
    w.add_tag("ui");
    w.add_tag("core");
    w.do_something();
    assert(w.name() == "panel");
    assert(w.tag_count() == 2);

    Widget w2 = std::move(w);
    w2.do_something();
    assert(w2.tag_count() == 2);

    std::cout << "benefits: change Impl members without recompiling Widget clients\n";
    std::cout << "costs: heap alloc + indirection; skip for tiny hot types\n";
    std::cout << "trap: defining ~Widget in the header while Impl is incomplete is ill-formed\n";
    (void)sizeof(FatInlineWidget);
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/pimpl_idiom", run>;

}  // namespace
