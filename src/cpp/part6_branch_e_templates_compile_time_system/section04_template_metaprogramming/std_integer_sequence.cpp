// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E4 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : std_integer_sequence
// Topic id : part6/e/section04/std_integer_sequence
//
// 要点: integer_sequence / index_sequence 是 pack 索引生成器;
//       用于 tuple 展开、数组到 pack、编译期索引算法。
// 参考: [intseq.intseq] cppreference integer_sequence

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>
#include <tuple>
#include <utility>

namespace {

// 用 index_sequence 展开 tuple
template <typename Tuple, std::size_t... I>
void print_tuple_impl(const Tuple& t, std::index_sequence<I...>) {
    ((std::cout << (I == 0 ? "" : ", ") << std::get<I>(t)), ...);
}

template <typename... Ts>
void print_tuple(const std::tuple<Ts...>& t) {
    std::cout << "  (";
    print_tuple_impl(t, std::index_sequence_for<Ts...>{});
    std::cout << ")\n";
}

// 数组 → pack 求和
template <typename T, std::size_t N, std::size_t... I>
constexpr T sum_array_impl(const std::array<T, N>& a, std::index_sequence<I...>) {
    return (a[I] + ...);
}

template <typename T, std::size_t N>
constexpr T sum_array(const std::array<T, N>& a) {
    return sum_array_impl(a, std::make_index_sequence<N>{});
}

// 反转 index_sequence
template <typename Seq>
struct reverse_seq;

template <std::size_t... I>
struct reverse_seq<std::index_sequence<I...>> {
    // 生成 N-1-I
    static constexpr std::size_t N = sizeof...(I);
    using type = std::index_sequence<(N - 1 - I)...>;
};

template <typename Tuple, std::size_t... I>
auto reverse_tuple_impl(Tuple&& t, std::index_sequence<I...>) {
    return std::make_tuple(std::get<I>(std::forward<Tuple>(t))...);
}

template <typename... Ts>
auto reverse_tuple(const std::tuple<Ts...>& t) {
    using Rev = typename reverse_seq<std::index_sequence_for<Ts...>>::type;
    return reverse_tuple_impl(t, Rev{});
}

// integer_sequence 自定义整数类型
template <int... Ns>
constexpr int product(std::integer_sequence<int, Ns...>) {
    return (1 * ... * Ns);
}

// select by indices
template <typename Tuple, std::size_t... I>
auto select(const Tuple& t, std::index_sequence<I...>) {
    return std::make_tuple(std::get<I>(t)...);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E4 std::integer_sequence ===\n";

    auto t = std::make_tuple(1, std::string{"x"}, 3.5);
    print_tuple(t);

    constexpr std::array<int, 4> a{1, 2, 3, 4};
    static_assert(sum_array(a) == 10);
    assert(sum_array(a) == 10);

    [[maybe_unused]] auto rev = reverse_tuple(std::make_tuple(1, 2, 3));
    assert(std::get<0>(rev) == 3 && std::get<2>(rev) == 1);

    static_assert(product(std::integer_sequence<int, 2, 3, 4>{}) == 24);

    [[maybe_unused]] auto picked = select(t, std::index_sequence<0, 2>{});
    assert(std::get<0>(picked) == 1);
    assert(std::get<1>(picked) == 3.5);

    // C++20: std::make_integer_sequence / index_sequence_for 常用
    using Seq = std::make_index_sequence<5>;
    static_assert(Seq::size() == 5);

    std::cout << "  tuple expand / array fold / reverse via index packs\n";
    std::cout << "std_integer_sequence: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/std_integer_sequence", run>;

}  // namespace
