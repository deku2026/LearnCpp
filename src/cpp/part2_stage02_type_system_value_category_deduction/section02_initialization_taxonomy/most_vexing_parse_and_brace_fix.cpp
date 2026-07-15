// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : most_vexing_parse_and_brace_fix
// Topic id : part2/stage02/section02/most_vexing_parse_and_brace_fix
//
// Covers: most vexing parse (declaration vs object), brace init fix

#include "learn/topic_registry.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

struct Widget {
    int id = 1;
    Widget() = default;
    explicit Widget(int v) : id(v) {}
    int get() const { return id; }
};

struct Holder {
    explicit Holder(Widget w) : value(w.get()) {}
    int value;
};

void demo_basics() {
    // Object construction with braces — never a function declaration.
    Widget w{};
    assert(w.get() == 1);

    Widget w2{42};
    assert(w2.get() == 42);
}

void demo_intermediate() {
    // Most vexing parse classic shape (as comments) and the brace fix.
    // Widget w();           // declares a function, not an object
    // Holder h(Widget());   // declares a function taking a function pointer

    Widget w{};          // object
    Holder h{Widget{}};  // object of Holder constructed from temporary Widget
    assert(w.get() == 1);
    assert(h.value == 1);

    Holder h2{Widget{9}};
    assert(h2.value == 9);
}

void demo_expert() {
    // Another common MVP: std::string vs function declaration
    // std::string s(std::string()); // function declaration in some contexts
    std::string s{std::string{"ok"}};
    assert(s == "ok");

    // istringstream construction
    std::istringstream input{"123"};
    int n = 0;
    input >> n;
    assert(n == 123);

    // Prefer braces when default-constructing to avoid function-declaration parse.
    struct Outer {
        Widget inner{};
    };
    Outer o{};
    assert(o.inner.get() == 1);

    // Direct parentheses construction of a type with no ambiguity
    Widget explicit_parens(7);
    assert(explicit_parens.get() == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/most_vexing_parse_and_brace_fix", run>;

}  // namespace
