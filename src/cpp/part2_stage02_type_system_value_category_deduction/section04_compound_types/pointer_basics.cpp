// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : pointer_basics
// Topic id : part2/stage02/section04/pointer_basics
//
// Covers: address-of, dereference, nullptr, pointer arithmetic on arrays

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>

namespace {

void demo_basics() {
    int x = 42;
    int* p = &x;
    assert(p != nullptr);
    assert(*p == 42);
    *p = 100;
    assert(x == 100);

    int* n = nullptr;
    assert(n == nullptr);
}

void demo_intermediate() {
    int arr[5] = {10, 20, 30, 40, 50};
    int* p = arr;  // array decays to pointer to first element
    assert(*p == 10);
    assert(*(p + 1) == 20);
    assert(p[2] == 30);

    p += 2;
    assert(*p == 30);
    --p;
    assert(*p == 20);

    const std::ptrdiff_t diff = &arr[4] - &arr[0];
    assert(diff == 4);
}

void demo_expert() {
    int a = 1;
    int b = 2;
    int* p = &a;
    int** pp = &p;
    assert(**pp == 1);
    *pp = &b;
    assert(*p == 2);

    // void* can hold any object pointer; cast back to use
    void* raw = &a;
    int* back = static_cast<int*>(raw);
    assert(*back == 1);

    // Pointer to const vs const pointer
    const int c = 9;
    const int* pc = &c;
    assert(*pc == 9);

    int mutable_v = 3;
    int* const cp = &mutable_v;
    *cp = 4;
    assert(mutable_v == 4);
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
