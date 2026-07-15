// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : constructor_delegation
// Topic id : part2/stage04/section01/constructor_delegation
//
// Covers: delegating constructors, shared init path, no member init with delegate

#include "learn/topic_registry.hpp"

#include <string>

namespace {

class Rect {
    int w_ = 0;
    int h_ = 0;

public:
    Rect(int w, int h) : w_(w), h_(h) {}
    Rect() : Rect(0, 0) {}
    explicit Rect(int side) : Rect(side, side) {}

    int w() const { return w_; }
    int h() const { return h_; }
    int area() const { return w_ * h_; }
};

class Named {
    std::string name_;
    int id_ = 0;

public:
    Named(std::string name, int id) : name_(std::move(name)), id_(id) {}
    explicit Named(std::string name) : Named(std::move(name), 0) {}

    const std::string& name() const { return name_; }
    int id() const { return id_; }
};

void demo_basics() {
    Rect a;
    LEARN_CHECK(a.w() == 0 && a.h() == 0);

    Rect b{4};
    LEARN_CHECK(b.w() == 4 && b.h() == 4);
    LEARN_CHECK(b.area() == 16);

    Rect c{3, 5};
    LEARN_CHECK(c.area() == 15);
}

void demo_intermediate() {
    Named n{"widget"};
    LEARN_CHECK(n.name() == "widget");
    LEARN_CHECK(n.id() == 0);

    Named m{"item", 7};
    LEARN_CHECK(m.id() == 7);
}

void demo_expert() {
    // A delegating ctor may not also initialize members in the same mem-init-list.
    // Body of the target ctor runs first, then the delegating ctor body.
    static int body_order = 0;
    body_order = 0;
    struct Trace {
        int* p;
        Trace(int* out, int tag) : p(out) { *p = tag; }
        Trace() : Trace(&body_order, 1) { body_order = 2; }
    };
    Trace t;
    LEARN_CHECK(body_order == 2);
    (void)t;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/constructor_delegation", run>;

}  // namespace
