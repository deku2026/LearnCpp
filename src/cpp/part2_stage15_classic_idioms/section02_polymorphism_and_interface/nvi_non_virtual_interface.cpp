// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : nvi_non_virtual_interface
// Topic id : part2/stage15/section02/nvi_non_virtual_interface
//
// Covers: NVI: public non-virtual, private virtual

#include "learn/topic_registry.hpp"

#include <memory>

namespace {

class Shape {
public:
    int area() const { return area_impl(); }
    virtual ~Shape() = default;

private:
    virtual int area_impl() const = 0;
};

class Square : public Shape {
    int side_;

public:
    explicit Square(int s) : side_(s) {}

private:
    int area_impl() const override { return side_ * side_; }
};

void demo_basics() {
    Square s(3);
    LEARN_CHECK(s.area() == 9);
}

void demo_intermediate() {
    std::unique_ptr<Shape> p = std::make_unique<Square>(4);
    LEARN_CHECK(p->area() == 16);
}

void demo_expert() {
    // NVI lets base enforce pre/post around customization.
    class Checked : public Shape {
        int area_impl() const override { return 5; }
    };
    Checked c;
    LEARN_CHECK(c.area() == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/nvi_non_virtual_interface", run>;

}  // namespace
