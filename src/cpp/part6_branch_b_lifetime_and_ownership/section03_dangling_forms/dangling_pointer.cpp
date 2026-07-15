// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_pointer
// Topic id : part6/b/section03/dangling_pointer
//
// Covers: dangling pointer teaching via safe contrasts; never use freed/out-of-scope addresses

#include "learn/topic_registry.hpp"

#include <memory>

namespace {

void demo_basics() {
    int x = 1;
    int* p = &x;
    LEARN_CHECK(*p == 1);
    // After x goes out of scope, p would dangle — we do not use that.
}

void demo_intermediate() {
    auto up = std::make_unique<int>(5);
    int* raw = up.get();
    LEARN_CHECK(*raw == 5);
    up.reset();
    // raw is dangling here — do not dereference. Track validity via owner.
    LEARN_CHECK(up.get() == nullptr);
}

void demo_expert() {
    // SAFE: null after release, or use smart pointers exclusively.
    int* p = new int(3);
    LEARN_CHECK(*p == 3);
    delete p;
    p = nullptr;
    LEARN_CHECK(p == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_pointer", run>;

}  // namespace
