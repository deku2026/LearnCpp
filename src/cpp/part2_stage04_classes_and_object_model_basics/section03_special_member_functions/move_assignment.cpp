// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : move_assignment
// Topic id : part2/stage04/section03/move_assignment
//
// Covers: move assignment releases old resource, transfers ownership

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace {

class Handle {
    int* p_ = nullptr;

public:
    explicit Handle(int v) : p_(new int(v)) {}
    ~Handle() { delete p_; }

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    Handle(Handle&& other) noexcept : p_(std::exchange(other.p_, nullptr)) {}

    Handle& operator=(Handle&& other) noexcept {
        if (this != &other) {
            delete p_;
            p_ = std::exchange(other.p_, nullptr);
        }
        return *this;
    }

    bool empty() const { return p_ == nullptr; }
    int value() const { return *p_; }
};

void demo_basics() {
    Handle a{1};
    Handle b{2};
    a = std::move(b);
    assert(a.value() == 2);
    assert(b.empty());
}

void demo_intermediate() {
    Handle a{5};
    a = std::move(a);
    assert(!a.empty());
    assert(a.value() == 5);
}

void demo_expert() {
    Handle a{3};
    Handle b{4};
    Handle c{5};
    a = std::move(b);
    b = std::move(c);
    assert(a.value() == 4);
    assert(b.value() == 5);
    assert(c.empty());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/move_assignment", run>;

}  // namespace
