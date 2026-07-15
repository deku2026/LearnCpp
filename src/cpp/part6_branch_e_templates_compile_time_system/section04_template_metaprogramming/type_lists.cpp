// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : type_lists
// Topic id : part6/e/section04/type_lists
//
// Covers: type list basics

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

template <class...>
struct type_list {};

template <class List>
struct size;

template <class... Ts>
struct size<type_list<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <class List>
struct front;

template <class T, class... Ts>
struct front<type_list<T, Ts...>> {
    using type = T;
};

void demo_basics() {
    using L = type_list<int, double, char>;
    static_assert(size<L>::value == 3);
}

void demo_intermediate() {
    using L = type_list<int, double>;
    static_assert(std::is_same_v<front<L>::type, int>);
}

void demo_expert() {
    static_assert(size<type_list<>>::value == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/type_lists", run>;

}  // namespace
