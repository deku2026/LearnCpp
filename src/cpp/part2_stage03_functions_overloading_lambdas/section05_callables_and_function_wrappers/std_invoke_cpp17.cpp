// Topic     : std::invoke（C++17）统一调用
// Doc       : 第2部分-阶段3 · 步骤 7.2
// cppreference: https://en.cppreference.com/cpp/utility/functional/invoke
//
// 要点: 一套语法调用函数、成员函数指针、成员数据指针、lambda、仿函数；
//       泛型代码中「调用」的标准定义。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>

namespace {

int free_fn(int x) {
    return x * 2;
}

struct S {
    int mem = 42;
    int method(int x) const { return x + mem; }
    void set(int v) { mem = v; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_invoke_cpp17] ===\n";

    // -------------------------------------------------------------------------
    // §入门：普通可调用
    // -------------------------------------------------------------------------
    assert(std::invoke(free_fn, 5) == 10);
    auto lam = [](int a, int b) { return a + b; };
    assert(std::invoke(lam, 1, 2) == 3);
    std::cout << "[intro] invoke free function and lambda\n";

    // -------------------------------------------------------------------------
    // §进阶：成员函数 / 成员数据
    // -------------------------------------------------------------------------
    S s;
    assert(std::invoke(&S::method, s, 8) == 50);   // 对象
    assert(std::invoke(&S::method, &s, 8) == 50);  // 指针
    assert(std::invoke(&S::mem, s) == 42);
    std::invoke(&S::set, s, 7);
    assert(s.mem == 7);

    // 传统写法对照：(s.*pmf)(args) / s.*pmd
    auto pmf = &S::method;
    assert((s.*pmf)(1) == 8);
    std::cout << "[advanced] member pointers unified; no .* / ->* at call site\n";

    // -------------------------------------------------------------------------
    // §专家：引用包装、完美转发场景
    // -------------------------------------------------------------------------
    int n = 3;
    auto add_n = [](int x, int y) { return x + y; };
    assert(std::invoke(add_n, n, 4) == 7);

    // INVOKE 协议是 concept invocable / apply / bind_front 的基础
    static_assert(std::is_invocable_v<decltype(free_fn), int>);
    static_assert(std::is_invocable_r_v<int, decltype(&S::method), const S&, int>);

    std::string msg = "hi";
    auto append = [](std::string& s, char c) {
        s.push_back(c);
        return s.size();
    };
    assert(std::invoke(append, msg, '!') == 3);
    assert(msg == "hi!");

    std::cout << "[expert] INVOKE underpins invocable concepts and wrappers\n";
    std::cout << "=== std_invoke_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_invoke_cpp17", run>;

}  // namespace
