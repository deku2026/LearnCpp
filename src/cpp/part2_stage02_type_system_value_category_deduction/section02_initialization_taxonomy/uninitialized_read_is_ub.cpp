// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : uninitialized_read_is_ub
// Topic id : part2/stage02/section02/uninitialized_read_is_ub
//
// Covers: reading indeterminate scalar is UB; safe init patterns only

#include "learn/topic_registry.hpp"

#include <cstring>
#include <optional>
#include <vector>

namespace {

void demo_basics() {
    // Always initialize before read.
    int x = 0;
    LEARN_CHECK(x == 0);

    int y{};
    LEARN_CHECK(y == 0);

    double d = 0.0;
    LEARN_CHECK(d == 0.0);
}

void demo_intermediate() {
    // Arrays: initialize all elements you will read.
    int a[3]{};
    LEARN_CHECK(a[0] == 0 && a[1] == 0 && a[2] == 0);

    // Optional models "maybe no value" without indeterminate reads.
    std::optional<int> maybe;
    LEARN_CHECK(!maybe.has_value());
    maybe = 42;
    LEARN_CHECK(maybe.has_value());
    LEARN_CHECK(*maybe == 42);

    // Vector elements are initialized by the container.
    std::vector<int> v(4);
    LEARN_CHECK(v.size() == 4);
    LEARN_CHECK(v[0] == 0);
}

void demo_expert() {
    // Writing before reading is fine (still better to initialize at declaration).
    int buf[4];
    for (int& e : buf) {
        e = 1;
    }
    int sum = 0;
    for (int e : buf) {
        sum += e;
    }
    LEARN_CHECK(sum == 4);

    // memset of unsigned char / bytes is a common low-level init technique.
    unsigned char raw[8];
    std::memset(raw, 0, sizeof(raw));
    LEARN_CHECK(raw[0] == 0 && raw[7] == 0);

    // Pointers: initialize to nullptr, never read indeterminate pointer.
    int* p = nullptr;
    LEARN_CHECK(p == nullptr);
    int value = 5;
    p = &value;
    LEARN_CHECK(*p == 5);

    // Rule of thumb: if the language leaves a scalar indeterminate, do not read it.
    // Prefer {} value-init, constructors, or explicit assignment first.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/uninitialized_read_is_ub", run>;

}  // namespace
