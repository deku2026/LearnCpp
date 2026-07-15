// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section04_empty_base_and_no_unique_address
// Item     : empty_base_optimization
// Topic id : part6/a/section04/empty_base_optimization
//
// Covers: EBO: empty base may occupy zero size; standalone empty object size >= 1

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct Empty {};

struct Derived : Empty {
    int x = 0;
};

struct NotEmpty {
    Empty e;
    int x = 0;
};

void demo_basics() {
    static_assert(sizeof(Empty) >= 1);
    static_assert(sizeof(Derived) == sizeof(int));
    Derived d;
    d.x = 7;
    assert(d.x == 7);
}

void demo_intermediate() {
    // Empty member (without no_unique_address) usually costs at least 1 + padding.
    static_assert(sizeof(NotEmpty) > sizeof(int));
    assert(sizeof(Derived) < sizeof(NotEmpty));
}

void demo_expert() {
    // Classic use: empty allocator/deleter as base of container/unique_ptr.
    struct Policy {};
    struct Holder : Policy {
        void* p = nullptr;
    };
    static_assert(sizeof(Holder) == sizeof(void*));
    static_assert(std::is_empty_v<Empty>);
    static_assert(std::is_empty_v<Policy>);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section04/empty_base_optimization", run>;

}  // namespace
