// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E4 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : type_lists
// Topic id : part6/e/section04/type_lists
//
// 要点: 类型列表是 TMP 的基础数据结构 —— head/tail/push/map/filter/length。
// 参考: Modern C++ Design (typelist); mp11 / brigand 思想

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <typename... Ts>
struct type_list {};

// length
template <typename List>
struct length;

template <typename... Ts>
struct length<type_list<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

// front
template <typename List>
struct front;

template <typename H, typename... T>
struct front<type_list<H, T...>> {
    using type = H;
};

// push_front
template <typename T, typename List>
struct push_front;

template <typename T, typename... Ts>
struct push_front<T, type_list<Ts...>> {
    using type = type_list<T, Ts...>;
};

// push_back via pack
template <typename List, typename T>
struct push_back;

template <typename... Ts, typename T>
struct push_back<type_list<Ts...>, T> {
    using type = type_list<Ts..., T>;
};

// concat
template <typename A, typename B>
struct concat;

template <typename... As, typename... Bs>
struct concat<type_list<As...>, type_list<Bs...>> {
    using type = type_list<As..., Bs...>;
};

// map
template <template <typename> class F, typename List>
struct map;

template <template <typename> class F, typename... Ts>
struct map<F, type_list<Ts...>> {
    using type = type_list<typename F<Ts>::type...>;
};

// filter
template <template <typename> class Pred, typename List>
struct filter;

template <template <typename> class Pred>
struct filter<Pred, type_list<>> {
    using type = type_list<>;
};

template <template <typename> class Pred, typename H, typename... T>
struct filter<Pred, type_list<H, T...>> {
    using rest = typename filter<Pred, type_list<T...>>::type;
    using type = std::conditional_t<Pred<H>::value, typename push_front<H, rest>::type, rest>;
};

// contains
template <typename Needle, typename List>
struct contains : std::false_type {};

template <typename Needle, typename H, typename... T>
struct contains<Needle, type_list<H, T...>>
    : std::bool_constant<std::is_same_v<Needle, H> || contains<Needle, type_list<T...>>::value> {};

// demo transforms
template <typename T>
struct add_pointer {
    using type = T*;
};

template <typename T>
struct is_integral_pred : std::is_integral<T> {};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E4 type lists ===\n";

    using L = type_list<int, double, char>;
    static_assert(length<L>::value == 3);
    static_assert(std::is_same_v<front<L>::type, int>);

    using L2 = push_front<bool, L>::type;
    static_assert(std::is_same_v<L2, type_list<bool, int, double, char>>);

    using L3 = push_back<L, long>::type;
    static_assert(length<L3>::value == 4);

    using M = map<add_pointer, type_list<int, char>>::type;
    static_assert(std::is_same_v<M, type_list<int*, char*>>);

    using F = filter<is_integral_pred, type_list<int, double, long, void*>>::type;
    static_assert(std::is_same_v<F, type_list<int, long>>);

    static_assert(contains<double, L>::value);
    static_assert(!contains<float, L>::value);

    using C = concat<type_list<int>, type_list<char, bool>>::type;
    static_assert(std::is_same_v<C, type_list<int, char, bool>>);

    std::cout << "  length(L)=" << length<L>::value << " map/filter/concat OK\n";
    std::cout << "  modern alternative: constexpr tuples / mp11 / boost.mp11\n";
    std::cout << "type_lists: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/type_lists", run>;

}  // namespace
