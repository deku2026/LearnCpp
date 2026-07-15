// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : fold_expressions_cpp17
// Topic id : part2/stage06/section02/fold_expressions_cpp17
//
// Covers: unary/binary fold expressions (+, &&, ,, etc.)

#include "learn/topic_registry.hpp"

#include <string>
#include <utility>
#include <vector>

namespace {

template <typename... Ts>
constexpr auto sum(Ts... xs) {
    return (xs + ...);
}

template <typename... Ts>
constexpr bool all_true(Ts... xs) {
    return (xs && ...);
}

template <typename... Ts>
constexpr bool any_true(Ts... xs) {
    return (xs || ...);
}

template <typename T, typename... Ts>
void push_all(std::vector<T>& out, Ts&&... xs) {
    (out.push_back(std::forward<Ts>(xs)), ...);
}

void demo_basics() {
    static_assert(sum(1, 2, 3, 4) == 10);
    LEARN_CHECK(sum(1, 2, 3) == 6);
    LEARN_CHECK(all_true(true, true, true));
    LEARN_CHECK(!all_true(true, false, true));
}

void demo_intermediate() {
    LEARN_CHECK(any_true(false, false, true));
    LEARN_CHECK(!any_true(false, false));

    std::string s;
    auto append = [&](auto&&... parts) { ((s += parts), ...); };
    append("a", "b", "c");
    LEARN_CHECK(s == "abc");
}

void demo_expert() {
    std::vector<int> v;
    push_all(v, 1, 2, 3);
    LEARN_CHECK(v.size() == 3);
    LEARN_CHECK(v[0] == 1 && v[2] == 3);

    // Binary fold with init.
    auto sum_from = [](auto init, auto... xs) { return (init + ... + xs); };
    LEARN_CHECK(sum_from(10, 1, 2, 3) == 16);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/fold_expressions_cpp17", run>;

}  // namespace
