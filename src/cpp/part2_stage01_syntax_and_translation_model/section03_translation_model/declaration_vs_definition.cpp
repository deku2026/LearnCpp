// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : declaration_vs_definition
// Topic id : part2/stage01/section03/declaration_vs_definition
//
// Covers: declarations introduce names; definitions provide entities; one definition

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

// Multiple declarations of the same function are OK.
int scale(int x);
int scale(int x);  // redeclaration — same signature, still not a definition

// Exactly one definition in this TU (and program for non-inline external linkage).
int scale(int x) {
    return x * 2;
}

// Variable: declaration vs definition.
extern int g_config;  // declaration only (would need a definition somewhere to ODR-use)

// Provide the definition here so the program links (safe demo).
int g_config = 42;

// Class: the class definition declares members; out-of-line member needs a definition.
struct Widget {
    int id;
    int value() const;
};

int Widget::value() const {
    return id;
}

// Forward declaration of a class is a pure declaration (incomplete type).
struct Opaque;
Opaque* opaque_null();  // may return pointer to incomplete type

struct Opaque {
    int tag;
};

Opaque* opaque_null() {
    return nullptr;
}

void demo_basics() {
    // Call uses the declaration; linker needs the definition we provided above.
    assert(scale(21) == 42);

    // Variable definition allocates storage; declaration alone does not.
    assert(g_config == 42);
    g_config = 7;
    assert(g_config == 7);
}

void demo_intermediate() {
    // Redeclarations must match; definition body is unique.
    Widget w{11};
    assert(w.value() == 11);

    // Incomplete type: can declare pointers/references, not define objects until complete.
    Opaque* p = opaque_null();
    assert(p == nullptr);

    Opaque o{99};
    p = &o;
    assert(p->tag == 99);
}

void demo_expert() {
    // A definition is also a declaration. Not every declaration is a definition:
    //   - void f();              // declaration only
    //   - void f() { }           // definition
    //   - extern int x;          // declaration only
    //   - int x = 0;             // definition
    //   - struct S;              // declaration only
    //   - struct S { int n; };   // definition
    //
    // Rule of thumb: you may declare many times; you must define once (ODR)
    // for non-inline, non-template entities with external linkage.

    auto twice = [](int n) {
        // Nested declaration of scale is fine — still refers to the same function.
        int scale(int);
        return scale(n);
    };
    assert(twice(5) == 10);

    // Type alias is a declaration that does not define a new type entity.
    using Counter = int;
    Counter c = scale(3);
    assert(c == 6);

    std::string note = "declare often, define once";
    assert(note.find("define") != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/declaration_vs_definition", run>;

}  // namespace
