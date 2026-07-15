// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : thunks
// Topic id : part6/a/section02/thunks
//
// Covers: this-adjustment for non-primary base virtual calls (thunk concept, safe demo)

#include "learn/topic_registry.hpp"

#include <cstdint>

namespace {

struct Left {
    int L = 10;
    virtual int who() const { return L; }
    virtual ~Left() = default;
};

struct Right {
    int R = 20;
    virtual int who() const { return R; }
    virtual ~Right() = default;
};

struct Both : Left, Right {
    int M = 30;
    int who() const override { return L + R + M; }
};

void demo_basics() {
    Both b;
    Left* pl = &b;
    Right* pr = &b;
    // Virtual call through either base reaches Both::who after this adjustment.
    LEARN_CHECK(pl->who() == 60);
    LEARN_CHECK(pr->who() == 60);
}

void demo_intermediate() {
    Both b;
    Right* pr = &b;
    // Address of Right subobject may differ from complete object.
    LEARN_CHECK(static_cast<void*>(pr) == static_cast<void*>(&b) ||
                reinterpret_cast<std::uintptr_t>(pr) > reinterpret_cast<std::uintptr_t>(&b));
    LEARN_CHECK(static_cast<Both*>(pr) == &b);
}

void demo_expert() {
    // Teaching note: ABI may emit a thunk that subtracts the Right offset from
    // this before jumping to Both::who. We only observe correct behavior.
    Both b;
    Left& l = b;
    Right& r = b;
    LEARN_CHECK(l.who() == r.who());
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/thunks", run>;

}  // namespace
