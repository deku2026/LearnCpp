// Topic    : auto NTTP（C++17）—— 非类型参数类型自动推导
// Doc      : 第2部分-阶段6 · 步骤 3.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : auto_nttp_cpp17
// Topic id : part2/stage06/section03/auto_nttp_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/template_parameters
//            ISO [temp.param]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — template<auto N> 让 N 的类型从实参推导
// ---------------------------------------------------------------------------

template <auto N>
struct Constant {
    static constexpr auto value = N;
    using type = decltype(N);
};

// ---------------------------------------------------------------------------
// §进阶 — 函数模板 auto NTTP；混合类型值列表
// ---------------------------------------------------------------------------

template <auto V>
constexpr auto identity_v = V;

template <auto A, auto B>
constexpr auto sum_v = A + B;

template <auto... Vs>
struct ValuePack {
    static constexpr std::size_t size = sizeof...(Vs);
};

// 按值类型分派
template <auto N>
constexpr const char* kind() {
    using T = decltype(N);
    if constexpr (std::is_same_v<T, bool>) {
        return "bool";
    } else if constexpr (std::is_integral_v<T>) {
        return "integral";
    } else if constexpr (std::is_enum_v<T>) {
        return "enum";
    } else if constexpr (std::is_pointer_v<T>) {
        return "pointer";
    } else {
        return "other";
    }
}

enum class Flag : unsigned { Off = 0, On = 1 };

int g = 1;

// ---------------------------------------------------------------------------
// §专家 — auto 与具体类型 NTTP 对照；C++20 字面值类
// ---------------------------------------------------------------------------

template <int N>
struct FixedInt {
    static constexpr int value = N;
};

struct Ratio {
    int n = 1;
    int d = 1;
    constexpr bool operator==(const Ratio&) const = default;
};

template <auto R>
constexpr int scale(int x) {
    if constexpr (std::is_same_v<decltype(R), Ratio>) {
        return x * R.n / R.d;
    } else {
        return static_cast<int>(x * R);
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [auto_nttp_cpp17] 入门 ===\n";
    static_assert(Constant<42>::value == 42);
    static_assert(std::is_same_v<Constant<42>::type, int>);
    static_assert(std::is_same_v<Constant<'x'>::type, char>);
    static_assert(std::is_same_v<Constant<100u>::type, unsigned>);
    static_assert(Constant<true>::value == true);
    std::cout << "Constant types OK\n";

    std::cout << "=== 进阶：变量模板与 ValuePack ===\n";
    static_assert(identity_v<7> == 7);
    static_assert(sum_v<3, 4> == 7);
    static_assert(ValuePack<1, 'a', true>::size == 3);
    assert(std::string{kind<0>()} == "integral");
    assert(std::string{kind<true>()} == "bool");
    assert(std::string{kind<Flag::On>()} == "enum");
    assert(std::string{kind<&g>()} == "pointer");
    std::cout << "kind examples OK\n";

    std::cout << "=== 专家：与固定类型 NTTP / 字面值类 ===\n";
    static_assert(FixedInt<5>::value == 5);
    // auto 版不必写 template<long N> / template<char N> 多份
    static_assert(scale<2>(10) == 20);
    static_assert(scale<Ratio{1, 2}>(10) == 5);
    // Constant<42> 与 Constant<42L> 是不同类型（N 类型不同）
    static_assert(!std::is_same_v<Constant<42>, Constant<42L>>);
    std::cout << "=== auto_nttp_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/auto_nttp_cpp17", run>;

}  // namespace
