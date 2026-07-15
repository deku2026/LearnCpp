// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : parameter_packs
// Topic id : part2/stage06/section02/parameter_packs
//
// Covers: type/non-type/template parameter packs, pack as template params

#include "learn/topic_registry.hpp"

#include <tuple>
#include <type_traits>

namespace {

template <typename... Ts>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Ts);
};

template <int... Ns>
struct IntList {
    static constexpr std::size_t size = sizeof...(Ns);
};

template <typename... Ts>
std::tuple<Ts...> make_tuple_copy(Ts... xs) {
    return std::tuple<Ts...>(xs...);
}

void demo_basics() {
    static_assert(TypeList<int, double, char>::size == 3);
    static_assert(TypeList<>::size == 0);
    static_assert(IntList<1, 2, 3, 4>::size == 4);
    LEARN_CHECK(TypeList<int>::size == 1);
}

void demo_intermediate() {
    auto t = make_tuple_copy(1, 2.5, 'x');
    LEARN_CHECK(std::get<0>(t) == 1);
    LEARN_CHECK(std::get<1>(t) == 2.5);
    LEARN_CHECK(std::get<2>(t) == 'x');
    static_assert(std::tuple_size_v<decltype(t)> == 3);
}

void demo_expert() {
    static_assert(std::is_same_v<TypeList<int, int>, TypeList<int, int>>);
    auto empty = make_tuple_copy();
    static_assert(std::tuple_size_v<decltype(empty)> == 0);
    (void)empty;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/parameter_packs", run>;

}  // namespace
