// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : virtual_inheritance_diamond
// Topic id : part6/a/section02/virtual_inheritance_diamond
//
// Covers: diamond problem, virtual base shared once, most-derived initialization

#include "learn/topic_registry.hpp"

namespace {

struct VBase {
    int v = 0;
    virtual int id() const { return v; }
    virtual ~VBase() = default;
};

struct Left : virtual VBase {
    int left = 1;
};

struct Right : virtual VBase {
    int right = 2;
};

struct Diamond : Left, Right {
    int bottom = 3;
    Diamond() { v = 42; }
};

struct LeftNV {
    int base_data = 0;
};
struct RightNV {
    int base_data = 0;
};
// Non-virtual diamond would duplicate a shared base; virtual inheritance merges.

void demo_basics() {
    Diamond d;
    LEARN_CHECK(d.v == 42);
    LEARN_CHECK(d.left == 1 && d.right == 2 && d.bottom == 3);
}

void demo_intermediate() {
    Diamond d;
    Left* L = &d;
    Right* R = &d;
    VBase* from_L = L;
    VBase* from_R = R;
    // Single shared virtual base subobject.
    LEARN_CHECK(from_L == from_R);
    LEARN_CHECK(from_L->id() == 42);
}

void demo_expert() {
    Diamond d;
    LEARN_CHECK(static_cast<VBase*>(&d)->id() == 42);
    // Virtual bases typically cost extra size (vbase offsets); layout ABI-dependent.
    LEARN_CHECK(sizeof(Diamond) >= sizeof(int) * 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/virtual_inheritance_diamond", run>;

}  // namespace
