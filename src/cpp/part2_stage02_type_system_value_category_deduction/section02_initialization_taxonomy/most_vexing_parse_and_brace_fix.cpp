// Topic     : Most Vexing Parse 与 {} 消歧义
// Doc       : 第2部分-阶段2 · 步骤 2.4
// cppreference: https://en.cppreference.com/cpp/language/direct_initialization
//               https://en.wikipedia.org/wiki/Most_vexing_parse
//
// 要点: 能解析成函数声明的，就解析成函数声明；
//       Widget w(); 与 Widget w(Timer()); 都是声明；用 {} 或额外括号修复。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

struct Timer {
    int ticks{0};
};

struct Widget {
    int id{0};
    Widget() : id{1} {}
    explicit Widget(Timer t) : id{100 + t.ticks} {}
    int foo() const { return id; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [most_vexing_parse_and_brace_fix] ===\n";

    // -------------------------------------------------------------------------
    // 入门：空括号“假对象”
    // -------------------------------------------------------------------------
    // Widget w();  // 声明：Widget w(); 无参函数，返回 Widget —— 不是对象！
    Widget w{};  // 值初始化对象
    assert(w.foo() == 1);
    std::cout << "[intro] Widget w{} is an object; Widget w() would be a function\n";

    // -------------------------------------------------------------------------
    // 进阶：带临时量参数的 MVP
    // -------------------------------------------------------------------------
    // Widget w2(Timer());
    // 解析为：Widget w2(Timer (*)());  — 参数是“返回 Timer 的函数”指针
    // 因此 w2.foo() 无法编译。

    Widget w2{Timer{}};  // 明确：用临时 Timer 构造对象
    assert(w2.foo() == 100);

    // 老式消歧义：多一层括号
    Widget w3((Timer()));
    assert(w3.foo() == 100);

    // 具名临时
    Timer t{};
    t.ticks = 5;
    Widget w4(t);  // 这里参数是左值，不会触发 MVP
    assert(w4.foo() == 105);
    std::cout << "[advanced] brace or extra parens fix Timer() MVP\n";

    // -------------------------------------------------------------------------
    // 专家：在类型系统中的痕迹
    // -------------------------------------------------------------------------
    // 若真的写出函数声明，其类型是函数类型
    using Fn = Widget();
    static_assert(std::is_function_v<Fn>);

    // 函数指针 vs 对象
    Widget (*fp)() = nullptr;
    assert(fp == nullptr);

    // 现代默认：局部对象一律 T obj{}; 或 T obj{args}; 需要 (count,value) 时再对 vector 用 ()
    std::cout << "[expert] grammar prefers function declaration — braces cannot declare fns\n";
    std::cout << "=== most_vexing_parse_and_brace_fix: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/most_vexing_parse_and_brace_fix", run>;

}  // namespace
