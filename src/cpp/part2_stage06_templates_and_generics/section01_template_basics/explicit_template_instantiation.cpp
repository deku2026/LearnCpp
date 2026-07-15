// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : explicit_template_instantiation
// Topic id : part2/stage06/section01/explicit_template_instantiation
//
// Covers: explicit instantiation definition/declaration pattern (same TU demo)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

template <typename T>
struct Counter {
    static int created;
    T value{};
    Counter() { ++created; }
    explicit Counter(T v) : value(std::move(v)) { ++created; }
};

template <typename T>
int Counter<T>::created = 0;

// Explicit instantiation definition: force Counter<int> emission.
template struct Counter<int>;

template <typename T>
T twice(T x) {
    return x + x;
}

// Explicit instantiation of a function template.
template int twice<int>(int);
template double twice<double>(double);

void demo_basics() {
    Counter<int> a;
    Counter<int> b(7);
    assert(b.value == 7);
    assert(Counter<int>::created >= 2);
}

void demo_intermediate() {
    assert(twice(21) == 42);
    assert(twice(1.5) == 3.0);
    static_assert(std::is_same_v<decltype(twice(1)), int>);
    static_assert(std::is_same_v<decltype(twice(1.0)), double>);
}

void demo_expert() {
    // Different monomorph has its own static.
    Counter<std::string> s(std::string{"x"});
    assert(s.value == "x");
    assert(Counter<std::string>::created >= 1);
    // int counter is independent of string counter.
    const int before = Counter<int>::created;
    Counter<int> c;
    assert(Counter<int>::created == before + 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/explicit_template_instantiation", run>;

}  // namespace
