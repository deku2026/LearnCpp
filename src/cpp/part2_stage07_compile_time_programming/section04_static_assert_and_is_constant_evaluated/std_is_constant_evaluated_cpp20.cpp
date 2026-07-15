// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : std_is_constant_evaluated_cpp20
// Topic id : part2/stage07/section04/std_is_constant_evaluated_cpp20
//
// Refs:
//   https://en.cppreference.com/w/cpp/types/is_constant_evaluated
//   P0595R2 — std::is_constant_evaluated()
//   C++23: std::is_constant_evaluated 也在 <type_traits>；
//          语言级替代见 if consteval（section02）

#include "learn/topic_registry.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 检测「明显常量求值」(manifestly constant-evaluated)
// ---------------------------------------------------------------------------
// 返回 true 的典型上下文：
//   constexpr 变量初始化、static_assert 条件、数组边界、
//   模板实参、consteval 调用 等。
// 必须用普通 if，不能用 if constexpr。

constexpr int select_algorithm_id() {
    if (std::is_constant_evaluated()) {
        return 1;  // 编译期算法编号
    }
    return 2;  // 运行期算法编号
}

constexpr long long fib_ct(int n) {
    // 编译期友好：迭代
    long long a = 0;
    long long b = 1;
    for (int i = 0; i < n; ++i) {
        const long long next = a + b;
        a = b;
        b = next;
    }
    return a;
}

constexpr long long fib(int n) {
    if (std::is_constant_evaluated()) {
        return fib_ct(n);
    }
    // 运行期也可走同一实现；真实库或会换查表/快速倍增
    return fib_ct(n);
}

// ---------------------------------------------------------------------------
// §进阶 — 与构造函数、分配策略、字符串处理
// ---------------------------------------------------------------------------

struct BufferHint {
    int mode;  // 1=CT, 2=RT
    constexpr BufferHint() : mode(std::is_constant_evaluated() ? 1 : 2) {}
};

constexpr int digits_sum(int n) {
    if (std::is_constant_evaluated()) {
        int x = n < 0 ? -n : n;
        int s = 0;
        while (x > 0) {
            s += x % 10;
            x /= 10;
        }
        return s;
    }
    // 运行期：同样算法（示意可换成 locale/格式化库）
    int x = n < 0 ? -n : n;
    int s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

// 错误形态: if constexpr (std::is_constant_evaluated()) 在该上下文恒为 true（-Wconstant-evaluated）
// 正确形态: 普通 if (std::is_constant_evaluated()) 或 C++23 if consteval
// 教学用 if constexpr (true) 复现「else 被丢弃」；真写 ICE 会在 -Werror 下失败。
constexpr int footgun_marker(int x) {
    if constexpr (true /* 模拟 if constexpr (std::is_constant_evaluated()) */) {
        return x + 100;
    } else {
        return x;
    }
}

// ---------------------------------------------------------------------------
// §专家 — 不能直接调 consteval；与 if consteval 迁移；实现机制直觉
// ---------------------------------------------------------------------------
// 1) is_constant_evaluated 是 *表达式*，编译器在常量求值时把它替换为 true，
//    在运行期求值上下文生成 false（通常是运行期常量 false，可优化掉分支）。
// 2) 普通 if 的两个分支都要类型检查 / 可调用性检查 → 不能放 consteval 调用。
// 3) C++23 新代码优先 if consteval；保留本 API 是为了 C++20 代码与库兼容。
// 4) 在 *已是* 常量求值的子表达式里嵌套调用，行为要对照标准「明显」定义。

consteval int only_ct(int x) {
    return x + 1;
}

constexpr int migrate_ready(int x) {
    // C++23 写法（对照）：
    // if consteval { return only_ct(x); } else { return x + 1; }
    //
    // C++20 只能：
    if (std::is_constant_evaluated()) {
        return x + 1;  // 无法写 only_ct(x)
    }
    return x + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section04/std_is_constant_evaluated_cpp20 ===\n";

    constexpr int id_ct = select_algorithm_id();
    static_assert(id_ct == 1);
    // 经函数指针调用，阻止实现把整次调用提升为常量初始化（否则 ICE 恒 true）
    int (*select_rt)() = select_algorithm_id;
    int id_rt = select_rt();
    assert(id_rt == 2);
    std::cout << "[intro] algorithm id CT=" << id_ct << " RT=" << id_rt << '\n';

    constexpr long long f10 = fib(10);
    static_assert(f10 == 55);
    [[maybe_unused]] int n = 10;
    assert(fib(n) == 55);
    std::cout << "[intro] fib(10)=" << f10 << '\n';

    constexpr BufferHint hint_ct{};
    static_assert(hint_ct.mode == 1);
    // 同样经「运行期构造」避免常量初始化
    BufferHint* hint_heap = new BufferHint{};
    assert(hint_heap->mode == 2);
    const int hint_rt_mode = hint_heap->mode;
    delete hint_heap;
    BufferHint hint_rt{};  // 自动对象：多数实现为运行期路径
    (void)hint_rt;
    assert(hint_rt_mode == 2);
    std::cout << "[advanced] BufferHint mode CT=" << hint_ct.mode << " RT=" << hint_rt.mode << '\n';

    static_assert(digits_sum(12345) == 15);
    assert(digits_sum(12345) == 15);

    constexpr int fg_ct = footgun_marker(1);
    int one = 1;
    const int fg_rt = footgun_marker(one);
    static_assert(fg_ct == 101);
    assert(fg_rt == 101);
    std::cout << "[advanced] footgun if constexpr(ice): CT=" << fg_ct << " RT=" << fg_rt << " (else never taken)\n";

    static_assert(migrate_ready(4) == 5);
    // static_assert(only_ct(4) == 5); // OK：直接在立即/常量上下文
    std::cout << "[expert] is_constant_evaluated true-branch cannot call "
                 "consteval only_ct(param); use if consteval to migrate\n";

    // 直接展示 only_ct 在常量上下文可用
    constexpr int oc = only_ct(4);
    static_assert(oc == 5);
    (void)oc;

    std::cout << "std_is_constant_evaluated_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/std_is_constant_evaluated_cpp20", run>;

}  // namespace
