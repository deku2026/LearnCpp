// Topic    : 非类型模板参数 NTTP —— 值作参数、array 固定长度
// Doc      : 第2部分-阶段6 · 步骤 3.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : non_type_template_parameter
// Topic id : part2/stage06/section01/non_type_template_parameter
// Refs     : https://en.cppreference.com/w/cpp/language/template_parameters
//            ISO [temp.param]

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 整型 NTTP：编译期长度的 FixedArray
// ---------------------------------------------------------------------------

template <typename T, std::size_t N>
struct FixedArray {
    T data[N]{};

    constexpr std::size_t size() const noexcept { return N; }
    constexpr T& operator[](std::size_t i) { return data[i]; }
    constexpr const T& operator[](std::size_t i) const { return data[i]; }
};

// ---------------------------------------------------------------------------
// §进阶 — 指针/引用 NTTP；枚举；C++20 字面值类类型
// ---------------------------------------------------------------------------

enum class Color : int { Red = 1, Green = 2, Blue = 3 };

template <Color C>
struct ColorTag {
    static constexpr Color value = C;
};

int global_counter = 0;

// 指针 NTTP：必须指向有链接的对象（不能是字符串字面量）
template <int* P>
struct PtrHolder {
    static void bump() { ++(*P); }
    static int get() { return *P; }
};

// C++20：字面值类类型可作 NTTP（结构化相等）
struct Scale {
    int numerator = 1;
    int denominator = 1;
    constexpr bool operator==(const Scale&) const = default;
};

template <Scale S>
constexpr int apply_scale(int x) {
    return x * S.numerator / S.denominator;
}

// ---------------------------------------------------------------------------
// §专家 — N 参与类型身份；数组大小保留；与 std::array 对照
// ---------------------------------------------------------------------------

template <std::size_t N>
consteval std::size_t double_size() {
    return N * 2;
}

template <typename T, std::size_t N>
constexpr std::size_t extent_of(const T (&)[N]) {
    return N;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [non_type_template_parameter] 入门：FixedArray ===\n";
    FixedArray<int, 4> a{};
    a[0] = 10;
    a[1] = 20;
    static_assert(a.size() == 4);
    assert(a[0] == 10 && a[1] == 20);
    // FixedArray<int,4> 与 FixedArray<int,5> 是不同类型
    static_assert(!std::is_same_v<FixedArray<int, 4>, FixedArray<int, 5>>);
    std::cout << "FixedArray size=" << a.size() << '\n';

    std::cout << "=== 进阶：枚举 / 指针 / 字面值类 NTTP ===\n";
    static_assert(ColorTag<Color::Blue>::value == Color::Blue);
    PtrHolder<&global_counter>::bump();
    PtrHolder<&global_counter>::bump();
    assert(PtrHolder<&global_counter>::get() == 2);
    // ⚠️ 字符串字面量无链接，不能 template<const char* S = "hi">
    constexpr Scale half{1, 2};
    static_assert(apply_scale<half>(10) == 5);
    constexpr Scale three_halves{3, 2};
    static_assert(apply_scale<three_halves>(4) == 6);
    assert(apply_scale<three_halves>(4) == 6);
    std::cout << "apply_scale half(10)=" << apply_scale<half>(10) << '\n';

    std::cout << "=== 专家：N 进入类型系统；对照 std::array ===\n";
    static_assert(double_size<8>() == 16);
    [[maybe_unused]] int raw[3] = {1, 2, 3};
    assert(extent_of(raw) == 3);
    std::array<int, 3> sa{1, 2, 3};
    static_assert(sa.size() == 3);
    // NTTP 让「长度」成为类型的一部分，边界检查可在编译期做
    static_assert(FixedArray<char, 1>{}.size() == 1);
    std::cout << "=== non_type_template_parameter: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/non_type_template_parameter", run>;

}  // namespace
