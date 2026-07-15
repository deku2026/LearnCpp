// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : copy_initialization
// Topic id : part2/stage02/section02/copy_initialization
//
// Covers: T x = v, return-by-value, pass-by-value, explicit ctor rejection

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace {

struct ImplicitFromInt {
    ImplicitFromInt(int v) : value(v) {}
    int value;
};

struct ExplicitFromInt {
    explicit ExplicitFromInt(int v) : value(v) {}
    int value;
};

int identity(int x) {
    return x;
}

ImplicitFromInt make_implicit(int v) {
    return v;  // copy-init of return value
}

void demo_basics() {
    int a = 42;
    double d = 2.5;
    std::string s = "copy";
    assert(a == 42);
    assert(d == 2.5);
    assert(s == "copy");

    ImplicitFromInt x = 7;  // copy-init allows non-explicit converting ctor
    assert(x.value == 7);
}

void demo_intermediate() {
    // Function argument copy-initialization
    assert(identity(10) == 10);

    ImplicitFromInt y = make_implicit(11);
    assert(y.value == 11);

    // Copy-init from another object
    ImplicitFromInt z = y;
    assert(z.value == 11);

    // Braced copy-init still copy-init for T x = {v}
    ImplicitFromInt w = {12};
    assert(w.value == 12);

    // Explicit constructors cannot be used in copy-initialization:
    // ExplicitFromInt bad = 1; // ill-formed
    ExplicitFromInt ok(1);  // direct-init is fine
    assert(ok.value == 1);
}

void demo_expert() {
    // Copy elision may skip actual copies; observable value still correct.
    std::string s = std::string("elide");
    assert(s == "elide");

    // Move can be used in copy-initialization contexts.
    std::string src = "moved";
    std::string dst = std::move(src);
    assert(dst == "moved");

    // Array-to-pointer and function-to-pointer decays in copy-init of pointers.
    const char text[] = "abc";
    const char* p = text;
    assert(p[0] == 'a');

    int nums[] = {1, 2, 3};
    int* np = nums;
    assert(np[1] == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/copy_initialization", run>;

}  // namespace
