// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section07_ctad_inheritance_cpp23
// Item     : inherited_constructor_ctad_cpp23
// Topic id : part2/stage04/section07/inherited_constructor_ctad_cpp23
//
// Covers: C++23 CTAD through inherited constructors (P2582)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class T>
struct Base {
    T value;
    explicit Base(T v) : value(std::move(v)) {}
};

template <class T>
struct Derived : Base<T> {
    using Base<T>::Base;
};

// Portable CTAD: deduction guide (C++17). C++23 P2582 also enables CTAD from
// inherited constructors without an explicit guide on supporting compilers.
template <class T>
Derived(T) -> Derived<T>;

void demo_basics() {
    Derived<int> d{42};
    assert(d.value == 42);
}

void demo_intermediate() {
    Derived e{7};
    static_assert(std::is_same_v<decltype(e), Derived<int>>);
    assert(e.value == 7);
}

void demo_expert() {
    Derived<double> x{1.5};
    assert(x.value == 1.5);

    auto make = [](auto v) { return Derived{std::move(v)}; };
    auto y = make(std::string{"hi"});
    static_assert(std::is_same_v<decltype(y), Derived<std::string>>);
    assert(y.value == "hi");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section07/inherited_constructor_ctad_cpp23", run>;

}  // namespace
