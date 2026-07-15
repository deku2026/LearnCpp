// Topic    : 函数模板 —— 编译期单态化与鸭子类型
// Doc      : 第2部分-阶段6 · 步骤 1
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : function_template
// Topic id : part2/stage06/section01/function_template
// Refs     : https://en.cppreference.com/w/cpp/language/function_template
//            https://en.cppreference.com/w/cpp/language/templates
//            ISO [temp.fct]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 蓝图 + 单态化：每个具体类型生成一份独立函数
// ---------------------------------------------------------------------------
// template 本身不是可执行代码；max(3,5) 会实例化出 max<int>，
// max(2.5,1.5) 再实例化出完全另一份 max<double>。

template <typename T>
T larger(T a, T b) {
    return a > b ? a : b;
}

// typename / class 在类型参数处完全等价（历史原因，现代代码多用 typename）
template <class T>
T smaller(T a, T b) {
    return a < b ? a : b;
}

// ---------------------------------------------------------------------------
// §进阶 — 多参数、显式指定、返回类型与 common_type
// ---------------------------------------------------------------------------

// 两个实参类型可以不同：用 common_type 决定结果类型
template <typename T, typename U>
auto mix_max(T a, U b) -> std::common_type_t<T, U> {
    using R = std::common_type_t<T, U>;
    return static_cast<R>(a) > static_cast<R>(b) ? static_cast<R>(a) : static_cast<R>(b);
}

// 非推导位置：仅靠返回类型无法从调用推导 T → 必须显式写 larger_of_zero<double>()
template <typename T>
T larger_of_zero() {
    return T{};
}

// 完美转发风格：保留值类别（与阶段 2/5 同源）
template <typename T>
constexpr T&& identity_fwd(T&& x) noexcept {
    return std::forward<T>(x);
}

// ---------------------------------------------------------------------------
// §专家 — 鸭子类型边界、显式实例化点、与重载共存
// ---------------------------------------------------------------------------

struct Meter {
    int value = 0;
    // 故意只支持 < 比较；> 通过下面 free function 补齐
    friend bool operator<(Meter a, Meter b) { return a.value < b.value; }
    friend bool operator>(Meter a, Meter b) { return b < a; }
};

// 函数模板可以和普通函数重载共存：非模板对精确匹配优先
int abs_score(int x) {
    return x < 0 ? -x : x;
}

template <typename T>
T abs_score(T x) {
    return x < T{} ? -x : x;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [function_template] 入门：单态化 ===\n";
    assert(larger(3, 5) == 5);
    assert(larger(2.5, 1.5) == 2.5);
    assert(smaller(std::string{"zoo"}, std::string{"apple"}) == "apple");
    // 显式指定：强制走 string 比较，而不是 const char*
    assert(larger<std::string>("a", "b") == "b");
    std::cout << "larger(3,5)=" << larger(3, 5) << " larger(2.5,1.5)=" << larger(2.5, 1.5) << '\n';

    std::cout << "=== 进阶：异构实参 / 显式指定 ===\n";
    auto m = mix_max(3, 2.5);  // common_type_t<int,double> → double
    static_assert(std::is_same_v<decltype(m), double>);
    assert(std::abs(m - 3.0) < 1e-12);
    // 无实参可推导 → 必须写模板实参
    assert(larger_of_zero<int>() == 0);
    int live = 7;
    int& ref = identity_fwd(live);
    assert(&ref == &live);
    std::cout << "mix_max(3,2.5)=" << m << '\n';

    std::cout << "=== 专家：鸭子类型 + 与重载共存 ===\n";
    Meter a{3}, b{10};
    assert(larger(a, b).value == 10);
    // 精确匹配非模板 int 版优先；浮点走模板
    assert(abs_score(-4) == 4);
    assert(std::abs(abs_score(-2.5) - 2.5) < 1e-12);
    // ⚠️ larger(3, 2.5) 若用单参数 T 会推导失败（int 与 double 冲突）
    //    解法：larger<double>(3, 2.5) 或上面的 mix_max
    assert(larger<double>(3, 2.5) == 3.0);
    std::cout << "Meter larger value=" << larger(a, b).value << '\n';

    std::cout << "=== function_template: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/function_template", run>;

}  // namespace
