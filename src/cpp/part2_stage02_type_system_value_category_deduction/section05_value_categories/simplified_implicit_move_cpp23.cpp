// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : simplified_implicit_move_cpp23
// Topic id : part2/stage02/section05/simplified_implicit_move_cpp23
//
// Covers: return of local/id-expression may implicitly move; C++23 clarifications

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <version>

namespace {

struct Probe {
    int value = 0;
    bool moved_from = false;

    Probe() = default;
    explicit Probe(int v) : value(v) {}
    Probe(const Probe& o) : value(o.value) {}
    Probe(Probe&& o) noexcept : value(o.value), moved_from(false) {
        o.moved_from = true;
        o.value = -1;
    }
    Probe& operator=(const Probe& o) {
        value = o.value;
        moved_from = false;
        return *this;
    }
    Probe& operator=(Probe&& o) noexcept {
        value = o.value;
        moved_from = false;
        o.moved_from = true;
        o.value = -1;
        return *this;
    }
};

Probe make_by_return_local() {
    Probe local{42};
    return local;  // implicitly moves (not copy) in modern C++
}

Probe make_by_return_param(Probe p) {
    return p;  // by-value param: move candidates apply
}

void demo_basics() {
    Probe a = make_by_return_local();
    assert(a.value == 42);
    assert(!a.moved_from);

    std::string s = [] {
        std::string local = "hello";
        return local;  // implicit move into return value
    }();
    assert(s == "hello");
}

void demo_intermediate() {
    Probe in{7};
    Probe out = make_by_return_param(std::move(in));
    assert(out.value == 7);
    assert(in.moved_from);

    // Explicit move still works and is clear for non-return contexts
    Probe x{1};
    Probe y = std::move(x);
    assert(y.value == 1);
    assert(x.moved_from);
}

void demo_expert() {
#if defined(__cpp_implicit_move)
    // Feature test present on supporting implementations (C++23 wording work).
    static_assert(__cpp_implicit_move >= 202011L || __cpp_implicit_move < 202011L);
#endif

    // Returning a local rvalue-ref variable still moves when treated as xvalue appropriately;
    // prefer return by value for clarity.
    auto factory = []() -> Probe {
        Probe p{99};
        return p;
    };
    Probe z = factory();
    assert(z.value == 99);

    // NRVO/implicit move: either way, observer sees correct final value.
    auto build = [](int n) {
        Probe p{n};
        if (n > 0) {
            return p;
        }
        return Probe{0};
    };
    Probe pos = build(3);
    Probe zero = build(0);
    assert(pos.value == 3);
    assert(zero.value == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/simplified_implicit_move_cpp23", run>;

}  // namespace
