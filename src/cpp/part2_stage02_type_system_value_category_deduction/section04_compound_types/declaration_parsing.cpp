// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : declaration_parsing
// Topic id : part2/stage02/section04/declaration_parsing
//
// Covers: spiral/right-left rule, pointer-to-array vs array-of-pointer, function pointers

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

int add(int a, int b) {
    return a + b;
}
int mul(int a, int b) {
    return a * b;
}

void demo_basics() {
    int x = 1;
    int* p = &x;  // pointer to int
    int& r = x;   // reference to int
    assert(*p == 1);
    assert(r == 1);

    int arr[3] = {1, 2, 3};
    int* ap = arr;  // pointer to first element
    assert(ap[1] == 2);
}

void demo_intermediate() {
    int a = 1, b = 2, c = 3;
    int* pointers[3] = {&a, &b, &c};  // array of pointers
    assert(*pointers[0] == 1);
    assert(*pointers[2] == 3);

    int values[3] = {10, 20, 30};
    int (*ptr_to_array)[3] = &values;  // pointer to array of 3 int
    assert((*ptr_to_array)[1] == 20);

    static_assert(std::is_same_v<decltype(pointers), int* [3]>);
    static_assert(std::is_same_v<decltype(ptr_to_array), int (*)[3]>);
}

void demo_expert() {
    // Function pointer
    int (*fp)(int, int) = &add;
    assert(fp(2, 3) == 5);
    fp = mul;
    assert(fp(2, 3) == 6);

    // Array of function pointers
    int (*table[2])(int, int) = {add, mul};
    assert(table[0](4, 5) == 9);
    assert(table[1](4, 5) == 20);

    // const placement changes meaning
    int n = 7;
    const int* p1 = &n;  // pointer to const int
    int* const p2 = &n;  // const pointer to int
    assert(*p1 == 7);
    *p2 = 8;
    assert(n == 8);

    // Using aliases clarifies complex declarations
    using Func = int(int, int);
    using FuncPtr = Func*;
    FuncPtr f = add;
    assert(f(1, 1) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/declaration_parsing", run>;

}  // namespace
