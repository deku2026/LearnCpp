// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : injected_class_name
// Topic id : part6/d/section01/injected_class_name
//
// Covers: injected class name inside class scope

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

template <class T>
struct Box {
    using self = Box;  // injected-class-name Box means Box<T>
    T value{};
    Box* self_ptr() { return this; }
};

struct Node {
    Node* next = nullptr;
    int v = 0;
};

void demo_basics() {
    Node n;
    n.v = 1;
    assert(n.next == nullptr);
    assert(n.v == 1);
}

void demo_intermediate() {
    Box<int> b;
    b.value = 5;
    assert(b.self_ptr()->value == 5);
    static_assert(std::is_same_v<Box<int>::self, Box<int>>);
}

void demo_expert() {
    // In templates, injected-class-name can refer to the current specialization.
    Box<double> b;
    typename Box<double>::self copy = b;
    assert(copy.value == 0.0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/injected_class_name", run>;

}  // namespace
