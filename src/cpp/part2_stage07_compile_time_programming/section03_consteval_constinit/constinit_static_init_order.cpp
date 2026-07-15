// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : constinit_static_init_order
// Topic id : part2/stage07/section03/constinit_static_init_order
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/constinit
//   https://en.cppreference.com/w/cpp/language/initialization
//   P1143R2 — Adding the constinit keyword
//   ISO [dcl.constinit], [basic.start.static]
//   SIOF: Static Initialization Order Fiasco

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

namespace {

// ---------------------------------------------------------------------------
// §入门 — constinit：强制 *常量初始化*，但对象可变
// ---------------------------------------------------------------------------
// constinit 用于静态/线程存储期变量：
//   - 初始化必须是常量初始化，否则编译失败；
//   - 不蕴含 const —— 运行期仍可修改；
//   - 不能当作常量表达式使用（不能做数组边界/NTTP）。

constexpr int square(int n) {
    return n * n;
}

[[maybe_unused]] int twice_runtime(int n) {
    return n + n;
}

constinit int g_table_size = square(10);  // OK：编译期初始化为 100
// constinit int g_bad = twice_runtime(10); // 错误：无法常量初始化

// constexpr 变量：初始化 + 不可变
constexpr int kImmutable = square(3);

// ---------------------------------------------------------------------------
// §进阶 — SIOF 与 constinit 的解法（单 TU 模拟跨依赖）
// ---------------------------------------------------------------------------
// 跨翻译单元的命名空间作用域静态对象，动态初始化顺序未指定。
// 若 A 依赖 B 而 B 尚未动态初始化 → 读到零初始化值 → SIOF。
// 给 *被依赖方* 标 constinit → 强制常量初始化，早于任何动态初始化。

constexpr int compute_magic() {
    return 42;
}

// 被依赖方：编译期就绪
constinit int g_dependency = compute_magic();

// 依赖方：即使是动态初始化，读到的 g_dependency 也已是 42
int g_dependent = g_dependency;  // 动态初始化，但依赖已常量初始化

// 对比：若 g_dependency 是动态初始化且跨 TU，顺序不定。
// 本单文件无法复现链接顺序 bug，但语义模型相同。

// constinit 可变
void bump_table_size() {
    g_table_size += 1;
}

// ---------------------------------------------------------------------------
// §专家 — constinit vs constexpr；不要求常量析构
// ---------------------------------------------------------------------------
// constexpr 对象：常量初始化 + 常量析构 + const。
// constinit：只要求常量初始化；允许非 const、不要求常量析构。
// 因此某些「可常量构造但不可常量析构」的类型可以 constinit 而不能 constexpr。
// （shared_ptr 在标准上逐步放宽；这里用自定义类型演示区别。）

struct NonConstexprDtor {
    int value;
    constexpr NonConstexprDtor() noexcept : value(7) {}
    constexpr explicit NonConstexprDtor(int v) noexcept : value(v) {}
    // 非 constexpr 析构 → 不能做 constexpr 静态对象
    ~NonConstexprDtor() { value = -1; }
};

// constexpr NonConstexprDtor g_cx; // 通常非法：析构非 constexpr
constinit NonConstexprDtor g_ci{11};  // OK：不要求常量析构

// 线程存储期也可 constinit
thread_local constinit int tls_slot = square(2);

// 不能：
// int arr[g_table_size]; // g_table_size 不是常量表达式

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section03/constinit_static_init_order ===\n";

    assert(g_table_size == 100);
    static_assert(kImmutable == 9);
    assert(g_dependency == 42);
    assert(g_dependent == 42);
    assert(g_ci.value == 11);
    assert(tls_slot == 4);

    std::cout << "[intro] constinit g_table_size=" << g_table_size << " constexpr kImmutable=" << kImmutable << '\n';

    bump_table_size();
    assert(g_table_size == 101);
    std::cout << "[advanced] after mutation g_table_size=" << g_table_size << " (constinit is NOT const)\n";

    // 证明不能当常量表达式：用运行期变量承接
    [[maybe_unused]] const int n = g_table_size;
    assert(n == 101);
    // constexpr int m = g_table_size; // 非法

    std::cout << "[advanced] SIOF model: constinit g_dependency=" << g_dependency
              << " ensures g_dependent=" << g_dependent << " regardless of dynamic init ordering\n";

    g_ci.value = 99;
    assert(g_ci.value == 99);
    tls_slot = 8;
    assert(tls_slot == 8);

    std::cout << "[expert] NonConstexprDtor can be constinit (value now " << g_ci.value
              << ") but not constexpr static; "
                 "constinit does not require constant destruction\n";

    std::cout << "[expert] cheat-sheet: constexpr=const+CTCE usable; "
                 "consteval=must CTCE function; "
                 "constinit=must constant-init storage, mutable, not CTCE value\n";

    std::cout << "constinit_static_init_order: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section03/constinit_static_init_order", run>;

}  // namespace
