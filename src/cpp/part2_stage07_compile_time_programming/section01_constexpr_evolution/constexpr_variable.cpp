// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_variable
// Topic id : part2/stage07/section01/constexpr_variable
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/constexpr
//   https://en.cppreference.com/w/cpp/language/constant_expression
//   https://en.cppreference.com/w/cpp/named_req/LiteralType
//   ISO [dcl.constexpr], [expr.const]

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — constexpr 变量 = 编译期就定值的常量
// ---------------------------------------------------------------------------
// constexpr 变量要求：
//   1) 类型是字面值类型 (LiteralType)
//   2) 立即初始化
//   3) 初始化的完整表达式是常量表达式
// 隐含 const（对象声明上的 constexpr 蕴含 const），因此之后不可赋值。
// 用途：数组边界、非类型模板实参、static_assert、switch case 等需要
// 「整型常量表达式 / 常量表达式」的位置。
//
// 对比 const：
//   const int n = runtime_value();  // 只读，值可运行期才确定
//   constexpr int k = ...;          // 必须编译期可知

constexpr int kSize = 10;
constexpr double kPi = 3.141592653589793;
constexpr char kTag = 'C';

// 编译期对象：字面值类型 + constexpr 构造
struct Point {
    int x;
    int y;
    constexpr Point(int xx, int yy) noexcept : x(xx), y(yy) {}
    constexpr int manhattan() const noexcept { return (x < 0 ? -x : x) + (y < 0 ? -y : y); }
};

constexpr Point kOrigin{0, 0};
constexpr Point kCorner{3, 4};

// 可用作数组边界 / NTTP
template <std::size_t N>
struct FixedBuffer {
    char data[N]{};
};

// ---------------------------------------------------------------------------
// §进阶 — 依赖链、聚合、隐式 const、不能当「运行期再算」的开关
// ---------------------------------------------------------------------------

// 常量表达式可以依赖其它 constexpr 变量 / 函数结果。
constexpr int kArea = kCorner.x * kCorner.y;
constexpr int kManhattan = kCorner.manhattan();

// 聚合字面值类型也可 constexpr 初始化（C++17 起更宽松）。
struct AggregateRgb {
    unsigned char r, g, b;
};
constexpr AggregateRgb kCyan{0, 255, 255};

// constexpr 变量是 const：下列写法非法（保留为注释）
// kSize = 11;

// const 整型 + 常量初始化在历史 C++ 中也可作数组边界，但语义更窄：
// 它不是「通用常量表达式对象」模型；现代代码优先 constexpr。
const int kLegacyBound = 4;
int legacy_stack[kLegacyBound]{};  // OK for integral const with ICE initializer

// 运行期 const —— 不能进 constexpr 上下文
int runtime_seed() {
    return 7;
}

// ---------------------------------------------------------------------------
// §专家 — 静态存储期、ODR-use、与 constinit/const 的边界
// ---------------------------------------------------------------------------
// 1) 命名空间作用域的 constexpr 变量隐含 inline (C++17 起对 static 成员
//    更明确；对命名空间作用域对象，constexpr 常与内部链接 / 头文件重复定义
//    策略一起考虑)。教学上记住：头文件里的 constexpr 对象要当心 ODR。
// 2) constexpr 变量要求「常量析构」——有非平凡 / 非 constexpr 析构的类型
//    不能做 constexpr 变量（那是 constinit 的用武之地之一）。
// 3) 地址：对 constexpr 变量取地址得到的是编译期已知的对象，但指针本身
//    只有在满足常量表达式规则时才能继续参与常量求值。
// 4) 浮点 constexpr 合法，但跨实现比较结果时要小心（本例用范围断言）。

constexpr const int* kSizeAddress = &kSize;  // 地址本身可出现在常量上下文

// 不可：
// constexpr std::string kBad{"hi"}; // 在 C++20 前不行；C++20 起 string 可
// constexpr 但作为命名空间作用域持久对象仍受 transient allocation 限制
// （动态缓冲不能「泄漏」出常量求值）。见 section05。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section01/constexpr_variable ===\n";

    // --- 入门验证 ---
    int stack_arr[kSize]{};
    static_assert(sizeof(stack_arr) / sizeof(stack_arr[0]) == 10);
    static_assert(kSize == 10);
    static_assert(kCorner.manhattan() == 7);
    static_assert(kOrigin.x == 0 && kOrigin.y == 0);

    FixedBuffer<kSize> buf{};
    static_assert(sizeof(buf.data) == 10);

    std::cout << "[intro] kSize=" << kSize << " kPi=" << kPi << " kTag=" << kTag << " manhattan(3,4)=" << kManhattan
              << '\n';

    // --- 进阶验证 ---
    static_assert(kArea == 12);
    static_assert(kCyan.g == 255);
    static_assert(std::is_const_v<std::remove_reference_t<decltype(kSize)>>);

    const int runtime_only = runtime_seed();
    assert(runtime_only == 7);
    // constexpr int bad = runtime_only; // 非法：初始化不是常量表达式
    (void)legacy_stack;

    std::array<int, kSize> std_arr{};
    assert(std_arr.size() == 10);

    std::cout << "[advanced] kArea=" << kArea << " cyan=(" << static_cast<int>(kCyan.r) << ','
              << static_cast<int>(kCyan.g) << ',' << static_cast<int>(kCyan.b) << ")\n";

    // --- 专家验证 ---
    static_assert(*kSizeAddress == 10);
    assert(kSizeAddress == &kSize);

    // constexpr 变量 vs 运行期 const
    const int just_const = runtime_seed() * 2;
    assert(just_const == 14);
    std::cout << "[expert] &kSize is stable; just_const(runtime)=" << just_const << " (const ≠ constexpr)\n";

    // 浮点：编译期可知，运行期打印
    static_assert(kPi > 3.14 && kPi < 3.15);
    std::cout << "[expert] constexpr double kPi is a compile-time constant\n";

    std::cout << "constexpr_variable: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_variable", run>;

}  // namespace
