// Topic     : auto 返回类型推导（C++14）与 decltype(auto)
// Doc       : 第2部分-阶段3 · 步骤 4.2–4.3
// cppreference: https://en.cppreference.com/cpp/language/auto
//               https://en.cppreference.com/cpp/language/decltype
//
// 要点: C++14 起函数可用 auto 从 return 推导；规则同 auto 变量（按值、丢顶层 const/引用）；
//       多条 return 须同一类型；调用前须见定义；完美转发返回用 decltype(auto)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

auto add(int a, int b) {
    return a + b;
}  // 推导为 int
auto pi() {
    return 3.14;
}  // double

// 要返回引用：auto& 或 decltype(auto)
int g_value = 100;
auto& ref_to_global() {
    return g_value;
}

const std::string& pick(bool first, const std::string& a, const std::string& b) {
    return first ? a : b;
}

// auto 返回会丢掉引用 → 变成拷贝
auto wrap_auto(bool first, const std::string& a, const std::string& b) {
    return pick(first, a, b);  // 推导为 std::string（拷贝）
}

// decltype(auto) 保留表达式值类别
decltype(auto) wrap_decltype_auto(bool first, const std::string& a, const std::string& b) {
    return pick(first, a, b);  // 仍为 const std::string&
}

template <class F, class... Args>
decltype(auto) invoke_and_forward(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [auto_return_type_deduction_cpp14] ===\n";

    // -------------------------------------------------------------------------
    // §入门：从 return 推导
    // -------------------------------------------------------------------------
    assert(add(2, 40) == 42);
    assert(pi() > 3.0 && pi() < 3.2);
    static_assert(std::is_same_v<decltype(add(1, 2)), int>);
    static_assert(std::is_same_v<decltype(pi()), double>);
    std::cout << "[intro] auto return deduces like auto variables (by value)\n";

    // -------------------------------------------------------------------------
    // §进阶：引用返回、多 return 一致性、可见性
    // -------------------------------------------------------------------------
    ref_to_global() = 7;
    assert(g_value == 7);
    static_assert(std::is_same_v<decltype(ref_to_global()), int&>);

    // 多条 return 必须同一推导类型：
    // auto bad(bool b) { if (b) return 1; else return 3.14; }  // ❌ int vs double

    // 仅有声明、不见定义时无法推导返回类型 → 不能只靠前向声明调用 auto 返回函数。
    std::cout << "[advanced] auto& keeps reference; multi-return types must match\n";

    // -------------------------------------------------------------------------
    // §专家：decltype(auto) 完美转发返回
    // -------------------------------------------------------------------------
    const std::string a = "alpha";
    const std::string b = "beta";

    auto copied = wrap_auto(true, a, b);
    static_assert(std::is_same_v<decltype(copied), std::string>);
    assert(copied == "alpha");
    // 修改 copied 不影响 a
    copied = "changed";
    assert(a == "alpha");

    decltype(auto) referred = wrap_decltype_auto(true, a, b);
    static_assert(std::is_same_v<decltype(referred), const std::string&>);
    assert(&referred == &a);

    auto lam = [](int x) -> int& {
        static int s = 0;
        s = x;
        return s;
    };
    int& r = invoke_and_forward(lam, 9);
    assert(r == 9);
    r = 11;
    assert(invoke_and_forward(lam, 0) == 0);  // 返回同一 static，现为 0
    invoke_and_forward(lam, 3);
    assert(r == 3);  // r 仍绑定 static s

    // 怎么选：普通返回值 → auto 或显式类型；保留引用/转发 → decltype(auto)；
    // 依赖参数且想写清楚 → 尾置 -> decltype(expr)。
    std::cout << "[expert] decltype(auto) preserves ref; auto would copy\n";
    std::cout << "=== auto_return_type_deduction_cpp14: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/auto_return_type_deduction_cpp14", run>;

}  // namespace
