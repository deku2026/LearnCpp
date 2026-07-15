// Topic    : 参数包 —— 模板/函数/NTTP 三种 pack
// Doc      : 第2部分-阶段6 · 步骤 7.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : parameter_packs
// Topic id : part2/stage06/section02/parameter_packs
// Refs     : https://en.cppreference.com/w/cpp/language/parameter_pack
//            ISO [temp.variadic]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 模板参数包：0 个或多个类型
// ---------------------------------------------------------------------------

template <typename... Types>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Types);
};

// 函数参数包
template <typename... Args>
std::size_t count_args(Args... /*args*/) {
    return sizeof...(Args);  // 或 sizeof...(args)，两者在此处等价
}

// ---------------------------------------------------------------------------
// §进阶 — 用 pack 构造 tuple；空包合法
// ---------------------------------------------------------------------------

template <typename... Ts>
auto make_tuple_copy(Ts... xs) {
    return std::tuple<Ts...>(xs...);
}

template <typename T, typename... Rest>
struct Head {
    using type = T;
};

// NTTP 包（C++17 auto...）
template <auto... Values>
struct ValueList {
    static constexpr std::size_t size = sizeof...(Values);
};

// ---------------------------------------------------------------------------
// §专家 — pack 是「语法实体」不能直接 typedef；需展开后使用
// ---------------------------------------------------------------------------

template <typename... Ts>
struct PackTraits {
    static constexpr bool empty = sizeof...(Ts) == 0;
    static constexpr bool all_integral = (std::is_integral_v<Ts> && ...);
};

// 继承包展开：mixin 组合
struct Loud {
    void hello() const { std::cout << "  Loud\n"; }
};
struct Soft {
    void hello() const { std::cout << "  Soft\n"; }
};

template <typename... Mixins>
struct Combined : Mixins... {
    using Mixins::hello...;  // C++17 包展开 using（若多重 hello 需更细处理）
};

// 单 mixin 演示更稳妥
template <typename M>
struct Wrap : M {};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [parameter_packs] 入门：TypeList / count_args ===\n";
    static_assert(TypeList<>::size == 0);
    static_assert(TypeList<int>::size == 1);
    static_assert(TypeList<int, double, char>::size == 3);
    assert(count_args() == 0);
    assert(count_args(1, 2.0, "x") == 3);
    std::cout << "count_args(1,2.0,\"x\")=" << count_args(1, 2.0, "x") << '\n';

    std::cout << "=== 进阶：tuple 与 NTTP 包 ===\n";
    auto t = make_tuple_copy(1, std::string{"hi"}, 3.5);
    assert(std::get<0>(t) == 1);
    assert(std::get<1>(t) == "hi");
    static_assert(std::is_same_v<Head<int, double, char>::type, int>);
    static_assert(ValueList<1, 2, 3>::size == 3);
    static_assert(ValueList<'a', 42>::size == 2);
    std::cout << "tuple + ValueList OK\n";

    std::cout << "=== 专家：pack 特性与空包 ===\n";
    static_assert(PackTraits<>::empty);
    static_assert(PackTraits<int, long>::all_integral);
    static_assert(!PackTraits<int, double>::all_integral);
    Wrap<Loud> w;
    w.hello();
    // pack 不能写成 using Pack = Ts...; 必须落在模式里展开
    std::cout << "=== parameter_packs: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/parameter_packs", run>;

}  // namespace
