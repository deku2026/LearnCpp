// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : pack_expansion
// Topic id : part2/stage06/section02/pack_expansion
//
// Covers: pack expansion in calls, initializers, and type lists

#include "learn/topic_registry.hpp"

#include <array>
#include <tuple>
#include <utility>
#include <vector>

namespace {

template <typename T>
T identity(T x) {
    return x;
}

template <typename... Ts>
auto map_identity(Ts... xs) {
    return std::tuple<Ts...>(identity(xs)...);
}

template <typename... Ts>
std::vector<int> to_int_vector(Ts... xs) {
    return std::vector<int>{static_cast<int>(xs)...};
}

template <typename F, typename... Ts>
void apply_each(F f, Ts&&... xs) {
    (f(std::forward<Ts>(xs)), ...);
}

void demo_basics() {
    auto t = map_identity(1, 2.5, 'a');
    LEARN_CHECK(std::get<0>(t) == 1);
    LEARN_CHECK(std::get<1>(t) == 2.5);
    LEARN_CHECK(std::get<2>(t) == 'a');
}

void demo_intermediate() {
    auto v = to_int_vector(1, 2, 3, 4);
    LEARN_CHECK(v.size() == 4);
    LEARN_CHECK(v[0] == 1 && v[3] == 4);

    int sum = 0;
    apply_each([&](int x) { sum += x; }, 1, 2, 3);
    LEARN_CHECK(sum == 6);
}

void demo_expert() {
    std::array<int, 3> a{10, 20, 30};
    auto t = std::apply([](auto... xs) { return std::tuple(xs...); }, a);
    LEARN_CHECK(std::get<0>(t) == 10);
    LEARN_CHECK(std::get<2>(t) == 30);

    int n = 0;
    apply_each([&](auto) { ++n; }, 1, "x", 3.0);
    LEARN_CHECK(n == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/pack_expansion", run>;

}  // namespace
