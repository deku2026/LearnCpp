// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : pointer_basics
// Topic id : part2/stage02/section04/pointer_basics
//
// Covers: address-of, dereference, nullptr, pointer arithmetic on arrays

#include "learn/topic_registry.hpp"

#include <cstddef>

namespace {

void demo_basics() {
    int x = 42;
    int* p = &x;
    LEARN_CHECK(p != nullptr);
    LEARN_CHECK(*p == 42);
    *p = 100;
    LEARN_CHECK(x == 100);

    int* n = nullptr;
    LEARN_CHECK(n == nullptr);
}

void demo_intermediate() {
    int arr[5] = {10, 20, 30, 40, 50};
    int* p = arr;  // array decays to pointer to first element
    LEARN_CHECK(*p == 10);
    LEARN_CHECK(*(p + 1) == 20);
    LEARN_CHECK(p[2] == 30);

    p += 2;
    LEARN_CHECK(*p == 30);
    --p;
    LEARN_CHECK(*p == 20);

    const std::ptrdiff_t diff = &arr[4] - &arr[0];
    LEARN_CHECK(diff == 4);
}

void demo_expert() {
    int a = 1;
    int b = 2;
    int* p = &a;
    int** pp = &p;
    LEARN_CHECK(**pp == 1);
    *pp = &b;
    LEARN_CHECK(*p == 2);

    // void* can hold any object pointer; cast back to use
    void* raw = &a;
    int* back = static_cast<int*>(raw);
    LEARN_CHECK(*back == 1);

    // Pointer to const vs const pointer
    const int c = 9;
    const int* pc = &c;
    LEARN_CHECK(*pc == 9);

    int mutable_v = 3;
    int* const cp = &mutable_v;
    *cp = 4;
    LEARN_CHECK(mutable_v == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/pointer_basics", run>;

}  // namespace
