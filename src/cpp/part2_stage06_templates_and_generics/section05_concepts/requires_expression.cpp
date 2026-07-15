// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : requires_expression
// Topic id : part2/stage06/section05/requires_expression
//
// Covers: requires-expression forms (simple, type, compound, nested)

#include "learn/topic_registry.hpp"

#include <concepts>
#include <string>
#include <utility>

namespace {

template <typename T>
concept Incrementable = requires(T x) {
    ++x;                                  // simple
    x++;                                  // simple
    { x + 1 } -> std::convertible_to<T>;  // compound
};

template <typename T>
concept HasValueType = requires {
    typename T::value_type;  // type requirement
};

template <typename T>
concept NestedOk = requires(T t) {
    requires sizeof(t) >= 1;  // nested requirement
};

template <typename T>
concept CallableWithInt = requires(T f) {
    { f(0) } -> std::convertible_to<int>;
};

void demo_basics() {
    static_assert(Incrementable<int>);
    static_assert(!Incrementable<std::string>);
    LEARN_CHECK(Incrementable<int>);
}

void demo_intermediate() {
    static_assert(HasValueType<std::string>);
    static_assert(!HasValueType<int>);
    static_assert(NestedOk<char>);
}

void demo_expert() {
    auto f = [](int x) { return x + 1; };
    static_assert(CallableWithInt<decltype(f)>);
    LEARN_CHECK(f(41) == 42);

    struct S {
        using value_type = int;
    };
    static_assert(HasValueType<S>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/requires_expression", run>;

}  // namespace
