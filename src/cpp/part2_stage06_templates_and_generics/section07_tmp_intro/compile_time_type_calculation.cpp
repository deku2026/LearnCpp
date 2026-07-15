// Topic    : 编译期类型计算 —— 类型→类型的元函数
// Doc      : 第2部分-阶段6 · 步骤 13.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : compile_time_type_calculation
// Topic id : part2/stage06/section07/compile_time_type_calculation
// Refs     : https://en.cppreference.com/w/cpp/header/type_traits
//            ISO [meta]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — remove_all_pointers：递归剥指针
// ---------------------------------------------------------------------------

template <typename T>
struct remove_all_pointers {
    using type = T;
};

template <typename T>
struct remove_all_pointers<T*> {
    using type = typename remove_all_pointers<T>::type;
};

template <typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

// ---------------------------------------------------------------------------
// §进阶 — type list：取头/尾、连接
// ---------------------------------------------------------------------------

template <typename... Ts>
struct type_list {};

template <typename List>
struct front;

template <typename H, typename... T>
struct front<type_list<H, T...>> {
    using type = H;
};

template <typename List>
struct pop_front;

template <typename H, typename... T>
struct pop_front<type_list<H, T...>> {
    using type = type_list<T...>;
};

template <typename L1, typename L2>
struct concat;

template <typename... A, typename... B>
struct concat<type_list<A...>, type_list<B...>> {
    using type = type_list<A..., B...>;
};

// ---------------------------------------------------------------------------
// §专家 — conditional 选择类型；映射 transform
// ---------------------------------------------------------------------------

template <typename List, template <typename> class Meta>
struct transform;

template <typename... Ts, template <typename> class Meta>
struct transform<type_list<Ts...>, Meta> {
    using type = type_list<typename Meta<Ts>::type...>;
};

template <typename T>
struct add_pointer {
    using type = T*;
};

template <bool B, typename T, typename F>
struct my_conditional {
    using type = T;
};
template <typename T, typename F>
struct my_conditional<false, T, F> {
    using type = F;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [compile_time_type_calculation] 入门 ===\n";
    static_assert(std::is_same_v<remove_all_pointers_t<int***>, int>);
    static_assert(std::is_same_v<remove_all_pointers_t<int>, int>);
    static_assert(std::is_same_v<remove_all_pointers_t<const int*>, const int>);
    std::cout << "remove_all_pointers OK\n";

    std::cout << "=== 进阶：type_list 操作 ===\n";
    using L = type_list<int, double, char>;
    using H = front<L>::type;
    using R = pop_front<L>::type;
    using L2 = concat<type_list<float>, L>::type;
    static_assert(std::is_same_v<H, int>);
    static_assert(std::is_same_v<R, type_list<double, char>>);
    static_assert(std::is_same_v<L2, type_list<float, int, double, char>>);
    std::cout << "type_list front/pop/concat OK\n";

    std::cout << "=== 专家：transform / conditional ===\n";
    using P = transform<type_list<int, char>, add_pointer>::type;
    static_assert(std::is_same_v<P, type_list<int*, char*>>);
    using C = my_conditional<true, int, double>::type;
    using D = my_conditional<false, int, double>::type;
    static_assert(std::is_same_v<C, int>);
    static_assert(std::is_same_v<D, double>);
    // 标准库 type_traits / tuple 元素操作 = 这类类型计算的产品化
    using Third = std::tuple_element_t<2, std::tuple<int, char, long>>;
    static_assert(std::is_same_v<Third, long>);
    std::cout << "=== compile_time_type_calculation: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/compile_time_type_calculation", run>;

}  // namespace
