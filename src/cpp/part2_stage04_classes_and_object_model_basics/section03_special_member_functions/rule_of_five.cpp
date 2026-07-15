// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : rule_of_five
// Topic id : part2/stage04/section03/rule_of_five
//
// Covers: Rule of 5 — dtor + copy + move for raw owning pointer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace {

class Box {
    int* p_ = nullptr;

public:
    explicit Box(int v) : p_(new int(v)) {}
    ~Box() { delete p_; }

    Box(const Box& o) : p_(o.p_ ? new int(*o.p_) : nullptr) {}
    Box& operator=(const Box& o) {
        if (this != &o) {
            int* fresh = o.p_ ? new int(*o.p_) : nullptr;
            delete p_;
            p_ = fresh;
        }
        return *this;
    }

    Box(Box&& o) noexcept : p_(std::exchange(o.p_, nullptr)) {}
    Box& operator=(Box&& o) noexcept {
        if (this != &o) {
            delete p_;
            p_ = std::exchange(o.p_, nullptr);
        }
        return *this;
    }

    bool empty() const { return p_ == nullptr; }
    int get() const { return *p_; }
};

void demo_basics() {
    Box a{1};
    Box b = a;
    assert(b.get() == 1);
    a = Box{2};
    assert(a.get() == 2);
}

void demo_intermediate() {
    Box a{5};
    Box b = std::move(a);
    assert(a.empty());
    assert(b.get() == 5);
}

void demo_expert() {
    Box a{3};
    Box b{4};
    b = std::move(a);
    assert(a.empty());
    assert(b.get() == 3);
    Box c = b;
    assert(c.get() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/rule_of_five", run>;

}  // namespace
