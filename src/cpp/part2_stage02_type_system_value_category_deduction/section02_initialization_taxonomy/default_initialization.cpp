// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : default_initialization
// Topic id : part2/stage02/section02/default_initialization
//
// Covers: default-init leaves scalars indeterminate; class default ctor; arrays

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

struct WithDefault {
    int value = 7;
};

struct EmptyCtor {
    int value;
    EmptyCtor() : value(99) {}
};

void demo_basics() {
    // Class types with default constructors are default-initialized safely.
    WithDefault w;
    assert(w.value == 7);

    EmptyCtor e;
    assert(e.value == 99);

    std::string s;  // default-init -> empty string
    assert(s.empty());
}

void demo_intermediate() {
    // Arrays of class type: each element default-initialized.
    WithDefault arr[3];
    assert(arr[0].value == 7);
    assert(arr[1].value == 7);
    assert(arr[2].value == 7);

    // Prefer value-init for scalars to avoid indeterminate values.
    int safe{};
    assert(safe == 0);

    // Explicit default member initializers participate in default construction.
    struct Counter {
        int n = 0;
        void bump() { ++n; }
    };
    Counter c;
    c.bump();
    assert(c.n == 1);
}

void demo_expert() {
    // Default-init of a type with a user-provided default constructor.
    static_assert(std::is_default_constructible_v<WithDefault>);
    static_assert(std::is_default_constructible_v<EmptyCtor>);
    static_assert(std::is_default_constructible_v<std::string>);

    // Scalars: do not default-init then read; always initialize.
    int x = 0;  // safe alternative to bare `int x;`
    assert(x == 0);

    // Dynamic storage: new T is default-init; new T() is value-init.
    auto* p = new WithDefault;
    assert(p->value == 7);
    delete p;

    auto* q = new int();  // value-init -> 0
    assert(*q == 0);
    delete q;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/default_initialization", run>;

}  // namespace
