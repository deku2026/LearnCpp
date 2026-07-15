// Topic    : std::forward 的推导基础：转发引用 + 引用折叠
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 4.4 / 4.5
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : std_forward_deduction_basis
// Topic id : part2/stage02/section06/std_forward_deduction_basis
// Refs     : https://en.cppreference.com/cpp/utility/forward
//            https://en.cppreference.com/cpp/language/reference
//            《Effective Modern C++》条款 25–28

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 记录最终落到哪条重载
enum class Path { Lvalue, Rvalue };

Path sink(std::string&) {
    return Path::Lvalue;
}
Path sink(std::string&&) {
    return Path::Rvalue;
}

// 错误示范：按值/右值引用「假转发」
template <class T>
Path bad_always_lvalue(T&& x) {
    // 表达式 x 是具名的 → 永远是 lvalue，哪怕 T&& 推导成 string&&
    return sink(x);
}

template <class T>
Path good_forward(T&& x) {
    return sink(std::forward<T>(x));
}

// 简化版 forward 语义（教学用，勿在生产替换 std::forward）
template <class T>
constexpr T&& my_forward(std::remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}
template <class T>
constexpr T&& my_forward(std::remove_reference_t<T>&& t) noexcept {
    static_assert(!std::is_lvalue_reference_v<T>, "cannot forward rvalue as lvalue");
    return static_cast<T&&>(t);
}

template <class T>
Path good_my_forward(T&& x) {
    return sink(my_forward<T>(x));
}

// 工厂：完美转发构造参数
template <class T, class... Args>
T make_perfect(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [std_forward_deduction_basis] 入门：具名参数丢失右值性 ===\n";
    {
        std::string s = "hi";

        // 左值传入：T = string&，T&& 折叠为 string&
        assert(bad_always_lvalue(s) == Path::Lvalue);
        assert(good_forward(s) == Path::Lvalue);

        // 右值传入：T = string，T&& = string&&，但表达式 x 仍是 lvalue 名字
        assert(bad_always_lvalue(std::string{"tmp"}) == Path::Lvalue);  // 丢了右值性！
        assert(good_forward(std::string{"tmp"}) == Path::Rvalue);       // forward 恢复

        std::cout << "[intro] named param is lvalue; forward restores category\n";
    }

    std::cout << "=== 进阶：forward 依赖「T 是否为引用」的模板实参 ===\n";
    {
        // 传入左值时 T=U&，std::forward<T>(x) → static_cast<U&>(x) → 仍左值
        // 传入右值时 T=U，  std::forward<T>(x) → static_cast<U&&>(x) → 右值
        std::string a = "a";
        assert(good_my_forward(a) == Path::Lvalue);
        assert(good_my_forward(std::string{"b"}) == Path::Rvalue);

        // 类型断言：forward 的返回类型
        int n = 0;
        using F1 = decltype(std::forward<int&>(n));
        using F2 = decltype(std::forward<int>(n));
        static_assert(std::is_same_v<F1, int&>);
        static_assert(std::is_same_v<F2, int&&>);

        std::cout << "[advanced] forward<T> uses T's reference-ness from deduction\n";
    }

    std::cout << "=== 专家：与 move 分工；make 完美转发 ===\n";
    {
        // move：无条件变成 xvalue（你承诺可以掏空）
        // forward：有条件，仅当原始实参是右值时才变成 xvalue
        std::string s = "data";
        [[maybe_unused]] Path p_move = sink(std::move(s));
        assert(p_move == Path::Rvalue);

        std::string t = "keep";
        // std::forward<std::string&>(t) 保持左值——适合「可能左可能右」的模板参数
        [[maybe_unused]] Path p_fwd = sink(std::forward<std::string&>(t));
        assert(p_fwd == Path::Lvalue);
        assert(t == "keep");

        auto s1 = make_perfect<std::string>(5, 'x');
        assert(s1 == "xxxxx");
        std::string base = "hello";
        auto s2 = make_perfect<std::string>(base);             // 拷贝
        auto s3 = make_perfect<std::string>(std::move(base));  // 移动
        assert(s2 == "hello" && s3 == "hello");

        std::cout << "[expert] move = unconditional xvalue; forward = conditional\n";
    }

    std::cout << "[std_forward_deduction_basis] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/std_forward_deduction_basis", run>;

}  // namespace
