// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : reference_collapsing
// Topic id : part2/stage02/section06/reference_collapsing
//
// Covers: T& & / T& && / T&& & -> T&; T&& && -> T&&; forwarding refs

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>

namespace {

template <class T>
struct TypeTag {
    using type = T;
};

template <class T>
TypeTag<T> deduce(T&&) {
    return {};
}

void demo_basics() {
    using R1 = int&;
    using R2 = int&&;

    static_assert(std::is_same_v<R1&, int&>);
    static_assert(std::is_same_v<R1&&, int&>);
    static_assert(std::is_same_v<R2&, int&>);
    static_assert(std::is_same_v<R2&&, int&&>);
}

void demo_intermediate() {
    int x = 1;

    // Forwarding reference: T&& where T is deduced
    auto t1 = deduce(x);  // T = int&
    auto t2 = deduce(2);  // T = int
    static_assert(std::is_same_v<decltype(t1)::type, int&>);
    static_assert(std::is_same_v<decltype(t2)::type, int>);

    // Reference collapsing in aliases
    using LRef = int&;
    LRef&& still_lref = x;
    static_assert(std::is_same_v<decltype(still_lref), int&>);
    assert(still_lref == 1);
}

void demo_expert() {
    // std::forward relies on collapsing
    auto forward_demo = [](auto&& arg) -> decltype(auto) {
        using A = decltype(arg);
        return std::forward<A>(arg);
    };

    int n = 5;
    decltype(auto) a = forward_demo(n);
    decltype(auto) b = forward_demo(5);
    static_assert(std::is_same_v<decltype(a), int&>);
    static_assert(std::is_same_v<decltype(b), int&&>);
    assert(a == 5);
    assert(b == 5);

    // remove_reference then add && is the move cast pattern
    using T = int&;
    using Moved = std::remove_reference_t<T>&&;
    static_assert(std::is_same_v<Moved, int&&>);

    int v = 9;
    Moved m = static_cast<Moved>(v);
    assert(m == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/reference_collapsing", run>;

}  // namespace
