// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : move_constructor
// Topic id : part2/stage04/section03/move_constructor
//
// Covers: move ctor steals resource, source left empty, noexcept

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
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
    Handle& operator=(Handle&&) = delete;

    bool empty() const { return p_ == nullptr; }
    int value() const { return *p_; }
};

void demo_basics() {
    Handle a{42};
    Handle b = std::move(a);
    assert(a.empty());
    assert(b.value() == 42);
}

void demo_intermediate() {
    Handle a{1};
    Handle b = std::move(a);
    Handle c = std::move(b);
    assert(a.empty() && b.empty());
    assert(c.value() == 1);
}

void demo_expert() {
    static_assert(std::is_nothrow_move_constructible_v<Handle>);
    Handle a{9};
    auto make = [](Handle h) { return h; };
    Handle b = make(std::move(a));
    assert(a.empty());
    assert(b.value() == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/move_constructor", run>;

}  // namespace
