// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : std_integer_sequence
// Topic id : part6/e/section04/std_integer_sequence
//
// Covers: std::integer_sequence / index_sequence

#include "learn/topic_registry.hpp"

#include <tuple>
#include <utility>

namespace {

template <class Tuple, std::size_t... I>
int sum_tuple_impl(const Tuple& t, std::index_sequence<I...>) {
    return (0 + ... + static_cast<int>(std::get<I>(t)));
}

template <class... Ts>
int sum_tuple(const std::tuple<Ts...>& t) {
    return sum_tuple_impl(t, std::index_sequence_for<Ts...>{});
}

void demo_basics() {
    static_assert(std::integer_sequence<int, 0, 1, 2>::size() == 3);
}

void demo_intermediate() {
    auto t = std::make_tuple(1, 2, 3);
    LEARN_CHECK(sum_tuple(t) == 6);
}

void demo_expert() {
    using Seq = std::make_index_sequence<4>;
    static_assert(Seq::size() == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/std_integer_sequence", run>;

}  // namespace
