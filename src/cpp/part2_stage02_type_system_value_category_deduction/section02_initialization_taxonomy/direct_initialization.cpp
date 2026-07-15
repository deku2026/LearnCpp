// Topic     : 直接初始化 direct initialization
// Doc       : 第2部分-阶段2 · 步骤 2.1
// cppreference: https://en.cppreference.com/cpp/language/direct_initialization
//
// 要点: T x(args); / T(args) / new T(args) / static_cast 等；
//       考虑 explicit 构造；允许窄化（与列表初始化不同）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Widget {
    int n;
    explicit Widget(int v) : n{v} {}
    Widget(const char* s) : n{static_cast<int>(std::char_traits<char>::length(s))} {}
};

struct OnlyCopy {
    int n;
    OnlyCopy(int v) : n{v} {}
};

[[maybe_unused]] int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [direct_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：括号直接初始化
    // -------------------------------------------------------------------------
    [[maybe_unused]] int n(42);
    [[maybe_unused]] double d(3.14);
    std::string s("hello");
    assert(n == 42 && d == 3.14 && s == "hello");

    // 允许窄化（可能警告）
    [[maybe_unused]] int truncated(static_cast<int>(3.9));
    assert(truncated == 3);
    std::cout << "[intro] T x(args) direct-init; narrowing allowed\n";

    // -------------------------------------------------------------------------
    // 进阶：explicit 与多参构造
    // -------------------------------------------------------------------------
    Widget w1(5);  // OK：direct-init 可调用 explicit
    // Widget w2 = 5; // ❌ copy-init 不考虑 explicit Widget(int)
    Widget w3("abc");  // 非 explicit 构造
    assert(w1.n == 5 && w3.n == 3);

    std::vector<int> v(3, 7);  // count + value，不是 initializer_list
    assert(v.size() == 3 && v[0] == 7 && v[2] == 7);

    // 函数风格转换 / 临时量
    [[maybe_unused]] OnlyCopy oc = OnlyCopy(9);  // 直接初始化临时量，再拷贝初始化 oc（可 elide）
    assert(oc.n == 9);

    // static_cast<T>(expr) 也是直接初始化语境
    [[maybe_unused]] auto x = static_cast<int>(3.7);
    assert(x == 3);
    std::cout << "[advanced] explicit OK in direct-init; vector(n,val) uses parens\n";

    // -------------------------------------------------------------------------
    // 专家：直接列表初始化 T{args} 是另一条路径（list-init）
    // -------------------------------------------------------------------------
    // T x(args)  — direct-init，走普通重载决议
    // T x{args}  — list-init，可能优先 initializer_list，并禁窄化
    Widget w4{11};  // list-init 调用 explicit Widget(int) 也可以（direct-list）
    assert(w4.n == 11);

    // 多参数 most vexing parse：T x(U()); 可能是函数声明——见 MVP 专题
    std::cout << "[expert] parens = classic direct-init; braces = list-init rules\n";
    std::cout << "=== direct_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/direct_initialization", run>;

}  // namespace
