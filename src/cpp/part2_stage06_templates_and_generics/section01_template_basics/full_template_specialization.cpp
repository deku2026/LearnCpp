// Topic    : 全特化 —— 为具体类型提供完整专门实现
// Doc      : 第2部分-阶段6 · 步骤 5.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : full_template_specialization
// Topic id : part2/stage06/section01/full_template_specialization
// Refs     : https://en.cppreference.com/w/cpp/language/template_specialization
//            ISO [temp.spec]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 类模板全特化：TypeName
// ---------------------------------------------------------------------------

template <typename T>
struct TypeName {
    static const char* get() { return "unknown"; }
};

template <>
struct TypeName<int> {
    static const char* get() { return "int"; }
};

template <>
struct TypeName<double> {
    static const char* get() { return "double"; }
};

template <>
struct TypeName<std::string> {
    static const char* get() { return "std::string"; }
};

// ---------------------------------------------------------------------------
// §进阶 — 函数模板全特化（可用，但重载通常更清晰）
// ---------------------------------------------------------------------------

template <typename T>
bool nearly_equal(T a, T b) {
    return a == b;
}

// 全特化：浮点用 epsilon
template <>
[[maybe_unused]] bool nearly_equal<double>(double a, double b) {
    return (a > b ? a - b : b - a) < 1e-9;
}

// 对 const char* 的全特化：按内容比，不比指针
template <>
bool nearly_equal<const char*>(const char* a, const char* b) {
    if (a == b) {
        return true;
    }
    if (!a || !b) {
        return false;
    }
    return std::strcmp(a, b) == 0;
}

// ---------------------------------------------------------------------------
// §专家 — 全特化是「独立定义」；成员可以只特化一部分
// ---------------------------------------------------------------------------

template <typename T>
struct Serializer {
    static std::string to_text(const T& v) { return std::to_string(v); }
};

// 完全替换：string 不走 to_string
template <>
struct Serializer<std::string> {
    static std::string to_text(const std::string& v) { return "\"" + v + "\""; }
};

// 只特化静态成员（主模板仍在）
template <typename T>
struct Limits {
    static constexpr T max_value = T{};
};

template <>
constexpr int Limits<int>::max_value = 2147483647;

int run(int /*argc*/, char** /*argv*/) {
    (void)nearly_equal("a", "b");

    std::cout << "=== [full_template_specialization] 入门：TypeName ===\n";
    assert(std::string{TypeName<int>::get()} == "int");
    assert(std::string{TypeName<double>::get()} == "double");
    assert(std::string{TypeName<char>::get()} == "unknown");
    assert(std::string{TypeName<std::string>::get()} == "std::string");
    std::cout << "TypeName<int>=" << TypeName<int>::get() << '\n';

    std::cout << "=== 进阶：函数全特化 ===\n";
    assert(nearly_equal(3, 3));
    assert(nearly_equal(1.0, 1.0 + 1e-12));
    assert(!nearly_equal(1.0, 1.1));
    [[maybe_unused]] const char* p = "hello";
    [[maybe_unused]] char buf[] = "hello";
    assert(nearly_equal(p, static_cast<const char*>(buf)));
    assert(!nearly_equal(static_cast<const char*>("a"), static_cast<const char*>("b")));
    std::cout << "nearly_equal specializations OK\n";

    std::cout << "=== 专家：Serializer / 成员特化 ===\n";
    assert(Serializer<int>::to_text(42) == "42");
    assert(Serializer<std::string>::to_text("hi") == "\"hi\"");
    static_assert(Limits<int>::max_value == 2147483647);
    // 全特化后 TypeName<int> 与主模板无继承关系，是平行定义
    static_assert(!std::is_base_of_v<TypeName<char>, TypeName<int>>);
    std::cout << "=== full_template_specialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/full_template_specialization", run>;

}  // namespace
