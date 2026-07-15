// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : placement_new_and_destroy
// Topic id : part6/c/section01/placement_new_and_destroy
//
// Covers: placement new construct in raw storage; explicit destructor

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <new>
#include <type_traits>

namespace {

struct Widget {
    int x;
    explicit Widget(int v) : x(v) {}
    ~Widget() { x = -1; }
};

void demo_basics() {
    alignas(Widget) unsigned char buf[sizeof(Widget)];
    Widget* w = new (buf) Widget(7);
    assert(w->x == 7);
    w->~Widget();
}

void demo_intermediate() {
    alignas(Widget) unsigned char buf[sizeof(Widget)];
    auto* w = new (buf) Widget(3);
    assert(w->x == 3);
    std::destroy_at(w);
}

void demo_expert() {
    static_assert(std::is_trivially_destructible_v<int>);
    alignas(int) unsigned char buf[sizeof(int)];
    int* p = new (buf) int(11);
    assert(*p == 11);
    // trivial dtor: destroy_at is a no-op side-effect-wise
    std::destroy_at(p);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/placement_new_and_destroy", run>;

}  // namespace
