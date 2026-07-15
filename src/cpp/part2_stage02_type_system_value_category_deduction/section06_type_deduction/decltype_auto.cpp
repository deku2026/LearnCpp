// Topic    : decltype(auto)：用 decltype 规则从初始值推导
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 6.3
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : decltype_auto
// Topic id : part2/stage02/section06/decltype_auto
// Refs     : https://en.cppreference.com/cpp/language/auto
//            https://en.cppreference.com/cpp/language/decltype
//            ISO [dcl.spec.auto]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 完美转发返回类型：若 f 返回引用，auto 会丢掉引用变成拷贝
template <class F, class... Args>
decltype(auto) call_forward(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

// 对比：按值 auto 返回会强制拷贝/丢引用
template <class F, class... Args>
auto call_by_value_auto(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

int g_counter = 0;

int& get_counter_ref() {
    return g_counter;
}
int get_counter_val() {
    return g_counter;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [decltype_auto] 入门：与 auto 对照 ===\n";
    {
        int a = 1;

        auto c0 = a;              // int —— 按值
        decltype(auto) c1 = a;    // int —— 未括号标识符 → 声明类型 int
        decltype(auto) c2 = (a);  // int& —— (a) 是 lvalue → T&

        static_assert(std::is_same_v<decltype(c0), int>);
        static_assert(std::is_same_v<decltype(c1), int>);
        static_assert(std::is_same_v<decltype(c2), int&>);

        ++c2;
        assert(a == 2);  // c2 是 a 的别名
        assert(c0 == 1 && c1 == 1);

        std::cout << "[intro] decltype(auto) c2=(a) aliases a; a=" << a << '\n';
    }

    std::cout << "=== 进阶：保留 const 引用；不能再叠 &/* ===\n";
    {
        const int ca = 9;
        const int& cra = ca;

        auto plain = cra;            // int
        decltype(auto) exact = cra;  // const int&
        static_assert(std::is_same_v<decltype(plain), int>);
        static_assert(std::is_same_v<decltype(exact), const int&>);
        assert(&exact == &ca);

        // 非法写法（注释示意）：
        // decltype(auto)& bad1 = cra;   // ❌ 必须是声明类型的唯一组成
        // decltype(auto)* bad2 = &ca;   // ❌

        decltype(auto) moved = std::move(ca);  // const int&&（xvalue）
        static_assert(std::is_same_v<decltype(moved), const int&&>);
        assert(moved == 9);

        std::cout << "[advanced] preserves const&; cannot write decltype(auto)&\n";
    }

    std::cout << "=== 专家：返回类型转发（预告阶段 3/6）===\n";
    {
        g_counter = 10;
        decltype(auto) r = call_forward(get_counter_ref);
        static_assert(std::is_same_v<decltype(r), int&>);
        r = 20;
        assert(g_counter == 20);

        auto r2 = call_by_value_auto(get_counter_ref);
        static_assert(std::is_same_v<decltype(r2), int>);
        r2 = 99;
        assert(g_counter == 20);  // 拷贝，不改原对象

        decltype(auto) v = call_forward(get_counter_val);
        static_assert(std::is_same_v<decltype(v), int>);
        assert(v == 20);

        // 字符串：返回引用避免不必要拷贝
        std::string s = "hello";
        auto get_s = [&]() -> std::string& { return s; };
        decltype(auto) sr = call_forward(get_s);
        static_assert(std::is_same_v<decltype(sr), std::string&>);
        sr += "!";
        assert(s == "hello!");

        std::cout << "[expert] decltype(auto) return keeps reference identity\n";
    }

    std::cout << "[decltype_auto] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/decltype_auto", run>;

}  // namespace
