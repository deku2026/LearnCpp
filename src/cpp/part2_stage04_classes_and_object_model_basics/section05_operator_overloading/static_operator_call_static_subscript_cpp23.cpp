// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : static_operator_call_static_subscript_cpp23
// Topic id : part2/stage04/section05/static_operator_call_static_subscript_cpp23
//
// Covers: C++23 static operator() and static operator[]

#include "learn/topic_registry.hpp"

#include <cstddef>

namespace {

#if defined(__cpp_static_call_operator) || (defined(__cplusplus) && __cplusplus >= 202302L)

struct Hash {
    static std::size_t operator()(int x) { return static_cast<std::size_t>(x) * 2654435761u; }
};

struct Table {
    static int operator[](int i) { return i * i; }
};

void demo_basics() {
    Hash h;
    LEARN_CHECK(h(2) != 0);
    LEARN_CHECK(Hash{}(3) == Hash::operator()(3));
}

void demo_intermediate() {
    LEARN_CHECK(Table{}[4] == 16);
    LEARN_CHECK(Table::operator[](5) == 25);
}

void demo_expert() {
    // Stateless callables can drop the implicit object parameter.
    auto apply = [](auto f, int x) { return f(x); };
    LEARN_CHECK(apply(Hash{}, 1) == Hash{}(1));
}

#else

struct Hash {
    std::size_t operator()(int x) const { return static_cast<std::size_t>(x) * 2654435761u; }
};

void demo_basics() {
    Hash h;
    LEARN_CHECK(h(2) != 0);
}

void demo_intermediate() {
    LEARN_CHECK(true);
}

void demo_expert() {
    LEARN_CHECK(true);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage04/section05/static_operator_call_static_subscript_cpp23", run>;

}  // namespace
