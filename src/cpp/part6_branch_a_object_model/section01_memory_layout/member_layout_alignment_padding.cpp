// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : member_layout_alignment_padding
// Topic id : part6/a/section01/member_layout_alignment_padding
//
// Covers: declaration order, alignment, padding, offsetof, reordering for size

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {

struct Bad {
    char c;
    int i;
    char d;
};

struct Good {
    int i;
    char c;
    char d;
};

struct A {
    char c;
    int i;
    short s;
};

struct alignas(16) Aligned {
    char c;
    int i;
};

void demo_basics() {
    static_assert(offsetof(A, c) == 0);
    static_assert(offsetof(A, i) == 4);
    static_assert(offsetof(A, s) == 8);
    static_assert(sizeof(A) == 12);
    static_assert(alignof(A) == alignof(int));
}

void demo_intermediate() {
    static_assert(sizeof(Bad) >= sizeof(Good));
    static_assert(sizeof(Good) == 8);
    static_assert(sizeof(Bad) == 12);
    assert(offsetof(Good, i) == 0);
    assert(offsetof(Good, c) == 4);
}

void demo_expert() {
    static_assert(alignof(Aligned) == 16);
    static_assert(sizeof(Aligned) % 16 == 0);
    Aligned arr[2]{};
    assert(reinterpret_cast<std::uintptr_t>(&arr[1]) % 16u == 0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/member_layout_alignment_padding", run>;

}  // namespace
