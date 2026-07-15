// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : consteval_immediate_functions
// Topic id : part2/stage07/section03/consteval_immediate_functions
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/consteval
//   P1073R3 — Immediate functions
//   ISO [dcl.constexpr] (consteval is specified alongside constexpr)

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>

namespace {

// ---------------------------------------------------------------------------
// §入门 — consteval = 立即函数：必须在编译期产生常量
// ---------------------------------------------------------------------------
// 与 constexpr（能编译期也能运行期）相对：consteval 禁止运行期调用。
// 用途：强制查表生成、强制校验、保证零运行期开销的配置解析等。

consteval int sqr(int n) {
    return n * n;
}

consteval int ipow(int base, int exp) {
    int r = 1;
    for (int i = 0; i < exp; ++i) {
        r *= base;
    }
    return r;
}

// ---------------------------------------------------------------------------
// §进阶 — 合法调用点；与 constexpr 对照；取地址限制
// ---------------------------------------------------------------------------

constexpr int maybe_sqr(int n) {
    return n * n;  // 可 CT 可 RT
}

// consteval 可用于构造 / 成员函数（非析构、非分配函数）
struct Cfg {
    int width;
    int height;
    consteval Cfg(int w, int h) : width(w), height(h) {}
    consteval int cells() const { return width * height; }
};

// 编译期生成表
consteval auto make_squares() {
    std::array<int, 8> a{};
    for (int i = 0; i < 8; ++i) {
        a[static_cast<std::size_t>(i)] = sqr(i);
    }
    return a;
}

constexpr auto kSquares = make_squares();

// ---------------------------------------------------------------------------
// §专家 — 规则细节
// ---------------------------------------------------------------------------
// 1) consteval 蕴含 inline；不能与 constexpr 同时写在同一函数上。
// 2) 不能是析构函数、分配/释放函数。
// 3) 可取地址，但指针不能逃逸出立即函数上下文（否则可能非良构）。
// 4) 立即函数的调用若出现在需要运行期的上下文 → 编译错误。
// 5) 从 constexpr 函数调用 consteval：仅当实参使整个调用成为常量 /
//    或处于立即上下文（如 if consteval）时合法。

consteval int checked_positive(int n) {
    if (n <= 0) {
        // 在常量求值中用非常量路径触发失败的常见手法：造非法表达式
        // 这里用简单约定：返回 -1 表示失败，调用方 static_assert
        return -1;
    }
    return n;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section03/consteval_immediate_functions ===\n";

    constexpr int a = sqr(5);
    static_assert(a == 25);
    static_assert(ipow(2, 10) == 1024);
    static_assert(Cfg{4, 5}.cells() == 20);
    static_assert(kSquares[5] == 25);
    static_assert(checked_positive(3) == 3);

    int arr[sqr(3)]{};  // 数组边界需要常量 → OK
    static_assert(sizeof(arr) / sizeof(arr[0]) == 9);

    std::cout << "[intro] sqr(5)=" << a << " ipow(2,10)=" << ipow(2, 10) << '\n';

    // constexpr 可运行期
    int x = 5;
    assert(maybe_sqr(x) == 25);
    // int bad = sqr(x); // 错误：x 非常量，sqr 是 consteval
    std::cout << "[advanced] maybe_sqr(runtime 5)=" << maybe_sqr(x) << "; sqr(x) would be ill-formed\n";

    std::cout << "[advanced] kSquares = [";
    for (std::size_t i = 0; i < kSquares.size(); ++i) {
        std::cout << kSquares[i] << (i + 1 < kSquares.size() ? "," : "");
    }
    std::cout << "]\n";

    constexpr Cfg cfg{16, 9};
    static_assert(cfg.width == 16);
    std::cout << "[expert] consteval Cfg{16,9}.cells()=" << cfg.cells()
              << " — object produced entirely at compile time\n";

    // 取地址：在立即上下文中使用 OK 的模式（直接调用更常见）
    // auto* p = &sqr; int y = p(3); // 典型非良构：调用非立即上下文
    std::cout << "[expert] consteval ⇒ must be compile-time; "
                 "use when you want the compiler to *reject* runtime calls\n";

    std::cout << "consteval_immediate_functions: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section03/consteval_immediate_functions", run>;

}  // namespace
