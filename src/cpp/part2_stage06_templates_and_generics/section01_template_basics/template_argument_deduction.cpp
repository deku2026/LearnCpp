// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : template_argument_deduction
// Topic id : part2/stage06/section01/template_argument_deduction
//
// Covers: by-value / ref / forwarding-ref deduction, array-to-pointer decay

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

template <typename T>
constexpr int tag_by_value(T) {
    if constexpr (std::is_const_v<T>) {
        return 1;
    } else if constexpr (std::is_pointer_v<T>) {
        return 2;
    } else {
        return 0;
    }
}

template <typename T>
constexpr bool is_const_ref(T&) {
    return std::is_const_v<T>;
}

template <typename T>
struct TypeTag {
    using type = T;
};

template <typename T>
TypeTag<T> deduce(T&&) {
    return {};
}

void demo_basics() {
    int x = 1;
    const int cx = 2;
    LEARN_CHECK(tag_by_value(x) == 0);   // T = int (top-level const stripped on by-value)
    LEARN_CHECK(tag_by_value(cx) == 0);  // still int for by-value param
    LEARN_CHECK(is_const_ref(cx) == true);
    LEARN_CHECK(is_const_ref(x) == false);
}

void demo_intermediate() {
    int arr[3] = {1, 2, 3};
    // By-value: array decays to pointer.
    LEARN_CHECK(tag_by_value(arr) == 2);

    auto t1 = deduce(42);      // T = int (rvalue)
    auto t2 = deduce(arr[0]);  // T = int& (lvalue)
    static_assert(std::is_same_v<decltype(t1)::type, int>);
    static_assert(std::is_same_v<decltype(t2)::type, int&>);
}

void demo_expert() {
    const int y = 9;
    auto t3 = deduce(y);  // T = const int&
    static_assert(std::is_same_v<decltype(t3)::type, const int&>);

    int z = 0;
    auto t4 = deduce(std::move(z));  // T = int
    static_assert(std::is_same_v<decltype(t4)::type, int>);

    // Partial explicit args: remaining deduced.
    auto id = [](auto v) { return v; };
    LEARN_CHECK(id(5) == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/template_argument_deduction", run>;

}  // namespace
