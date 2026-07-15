// Topic     : lambda 值捕获
// Doc       : 第2部分-阶段3 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [x] / [=] 把用到的变量拷贝进闭包；默认 operator() 为 const，副本只读；
//       跨作用域保存回调时优先值捕获，避免悬垂。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>

namespace {

std::function<int()> make_good() {
    int local = 42;
    return [local] { return local; };  // 闭包拥有副本
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [capture_by_value] ===\n";

    // -------------------------------------------------------------------------
    // §入门：[x] 与 [=]
    // -------------------------------------------------------------------------
    int a = 1;
    int b = 2;
    auto by_a = [a] { return a; };
    auto all = [=] { return a + b; };
    assert(by_a() == 1);
    assert(all() == 3);

    a = 100;
    assert(by_a() == 1);  // 捕获时已拷贝，外部改动不影响
    assert(all() == 3);
    std::cout << "[intro] value capture copies at lambda creation\n";

    // -------------------------------------------------------------------------
    // §进阶：默认只读；混合捕获
    // -------------------------------------------------------------------------
    // [a] 的副本在 const operator() 里不可改（见 mutable_lambda topic）
    std::string s = "hi";
    auto greet = [s](const std::string& name) { return s + ", " + name; };
    assert(greet("Ada") == "hi, Ada");

    int x = 1;
    int y = 2;
    auto mixed = [=, &y] { return x + y; };  // x 值，y 引用
    y = 20;
    assert(mixed() == 21);
    std::cout << "[advanced] [=] default by value; mix with &y\n";

    // -------------------------------------------------------------------------
    // §专家：跨作用域安全
    // -------------------------------------------------------------------------
    auto f = make_good();
    assert(f() == 42);

    // 大对象值捕获有拷贝成本；只读且生命周期覆盖 lambda 使用期时可用 const& 捕获。
    // 不可拷贝对象（unique_ptr）不能 [p]，需 init-capture move（见 init_capture topic）。
    std::cout << "[expert] value capture owns data → safe to store/return callable\n";
    std::cout << "=== capture_by_value: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_value", run>;

}  // namespace
