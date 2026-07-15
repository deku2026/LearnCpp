// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : inherited_ctor_ctad_cpp23
// Topic id : part2/stage06/section03/inherited_ctor_ctad_cpp23
//
// Covers: CTAD with inherited constructors (C++23 improvement + portable fallback)

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <version>

namespace {

template <typename T>
struct Base {
    T value;
    explicit Base(T v) : value(std::move(v)) {}
};

template <typename T>
struct Derived : Base<T> {
    using Base<T>::Base;  // inherit constructors
};

#if defined(__cpp_deduction_guides)  // always true with CTAD; C++23 improves inherited CTAD
// Explicit guide keeps older compilers happy for Derived CTAD.
template <typename T>
Derived(T) -> Derived<T>;
#endif

void demo_basics() {
    Derived d{42};
    LEARN_CHECK(d.value == 42);
    static_assert(std::is_same_v<decltype(d), Derived<int>>);
}

void demo_intermediate() {
    Derived ds{std::string{"hi"}};
    LEARN_CHECK(ds.value == "hi");
    static_assert(std::is_same_v<decltype(ds), Derived<std::string>>);
}

void demo_expert() {
    // Explicit type still works without relying on CTAD.
    Derived<double> dd(3.5);
    LEARN_CHECK(dd.value == 3.5);

#if defined(__cpp_lib_containers_ranges) || defined(__cpp_concepts)
    // Feature-test noise: inherited ctor + CTAD available on modern toolchains.
    static_assert(true);
#endif
    Base b{7};
    LEARN_CHECK(b.value == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/inherited_ctor_ctad_cpp23", run>;

}  // namespace
