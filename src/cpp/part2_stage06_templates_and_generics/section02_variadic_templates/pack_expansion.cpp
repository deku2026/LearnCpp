// Topic    : 包展开 pattern... —— 在类型/表达式/初始化列表中展开
// Doc      : 第2部分-阶段6 · 步骤 7.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : pack_expansion
// Topic id : part2/stage06/section02/pack_expansion
// Refs     : https://en.cppreference.com/w/cpp/language/parameter_pack
//            ISO [temp.variadic]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 函数实参列表展开
// ---------------------------------------------------------------------------

template <typename T>
T id(T x) {
    return x;
}

template <typename... Args>
auto call_ids(Args... args) {
    // id(args)... → id(a1), id(a2), ...
    return std::tuple{id(args)...};
}

// ---------------------------------------------------------------------------
// §进阶 — 类型模式展开；初始化；继承
// ---------------------------------------------------------------------------

template <typename... Ts>
using tuple_of_ptrs = std::tuple<Ts*...>;

template <typename... Ts>
std::vector<std::size_t> sizes_of() {
    return {sizeof(Ts)...};
}

template <typename T>
struct Trace {
    Trace() { std::cout << "  Trace<" << typeid(T).name() << ">\n"; }
};

template <typename... Ts>
struct TraceAll : Trace<Ts>... {
    TraceAll() : Trace<Ts>()... {}  // 基类初始化列表包展开
};

// ---------------------------------------------------------------------------
// §专家 — 嵌套模式、成对展开、lambda 捕获
// ---------------------------------------------------------------------------

template <typename F, typename... Args>
void for_each_arg(F f, Args&&... args) {
    // 逗号运算符展开：对每个参数调用 f
    (f(std::forward<Args>(args)), ...);
}

template <typename... Ts>
auto tie_as_const(const Ts&... xs) {
    return std::tuple<const Ts&...>(xs...);
}

// 多个 pack 同时展开时长度必须一致；zip 用 index_sequence 最清晰
template <typename TupleA, typename TupleB, std::size_t... I>
auto zip_impl(TupleA&& a, TupleB&& b, std::index_sequence<I...>) {
    return std::tuple{std::pair{std::get<I>(std::forward<TupleA>(a)), std::get<I>(std::forward<TupleB>(b))}...};
}

template <typename TupleA, typename TupleB>
auto zip(TupleA&& a, TupleB&& b) {
    constexpr std::size_t n = std::tuple_size_v<std::decay_t<TupleA>>;
    static_assert(n == std::tuple_size_v<std::decay_t<TupleB>>);
    return zip_impl(std::forward<TupleA>(a), std::forward<TupleB>(b), std::make_index_sequence<n>{});
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pack_expansion] 入门：实参展开 ===\n";
    auto t = call_ids(1, 2.5, std::string{"z"});
    assert(std::get<0>(t) == 1);
    assert(std::get<2>(t) == "z");
    std::cout << "call_ids tuple OK\n";

    std::cout << "=== 进阶：类型展开 / 基类展开 ===\n";
    using P = tuple_of_ptrs<int, double>;
    static_assert(std::is_same_v<std::tuple_element_t<0, P>, int*>);
    auto sz = sizes_of<char, int, double>();
    assert(sz.size() == 3 && sz[0] == 1);
    TraceAll<int, char> traces;
    (void)traces;

    std::cout << "=== 专家：for_each / zip 展开 ===\n";
    std::vector<int> seen;
    for_each_arg([&](auto x) { seen.push_back(static_cast<int>(x)); }, 1, 2, 3);
    assert((seen == std::vector<int>{1, 2, 3}));

    int a = 10, b = 20;
    [[maybe_unused]] auto refs = tie_as_const(a, b);
    assert(&std::get<0>(refs) == &a);

    auto z = zip(std::tuple{1, 2}, std::tuple{std::string{"a"}, std::string{"b"}});
    assert(std::get<0>(z).first == 1 && std::get<0>(z).second == "a");
    assert(std::get<1>(z).first == 2 && std::get<1>(z).second == "b");
    std::cout << "=== pack_expansion: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/pack_expansion", run>;

}  // namespace
