// Topic     : lambda 值捕获
// Doc       : 第2部分-阶段3 · 步骤 5.3 / 验收「各种捕获」
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [x] / [=] 拷贝用到的外部变量；值捕获默认 operator() 为 const，成员只读；
//       跨作用域保存回调时优先值捕获，避免悬垂。
//       Effective Modern C++ 条款 31：慎用默认捕获模式（[=]/[&] 易漏变量/悬垂）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::function<int()> make_good() {
    int local = 42;
    return [local] { return local; };  // 值捕获拥有副本
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [capture_by_value] ===\n";

    // -------------------------------------------------------------------------
    // §入门：[x] 与 [=]
    // -------------------------------------------------------------------------
    int a = 1;
    int b = 2;
    [[maybe_unused]] auto by_a = [a] { return a; };
    [[maybe_unused]] auto all = [=] { return a + b; };
    assert(by_a() == 1);
    assert(all() == 3);

    a = 100;
    b = 200;
    assert(by_a() == 1);  // 创建时已拷贝，外部改动不影响
    assert(all() == 3);
    std::cout << "[intro] value capture copies at lambda creation\n";

    // -------------------------------------------------------------------------
    // §进阶：默认只读、混合捕获
    // -------------------------------------------------------------------------
    // [a] 的副本在 const operator() 上不可改（见 mutable_lambda topic）
    std::string s = "hi";
    auto greet = [s](const std::string& name) { return s + ", " + name; };
    assert(greet("Ada") == "hi, Ada");

    int x = 1;
    int y = 2;
    [[maybe_unused]] auto mixed = [=, &y] { return x + y; };  // x 值，y 引用
    y = 20;
    assert(mixed() == 21);  // 1 + 20

    // 显式列出捕获通常比 [=] 更清晰（读者立刻知道依赖哪些变量）
    [[maybe_unused]] auto explicit_caps = [x, y] { return x * 10 + y; };
    assert(explicit_caps() == 30);  // y 已是 20：等等——值捕获在创建时拷贝
    // 上面 explicit_caps 在 y=20 之后创建，所以是 1*10+20=30
    std::cout << "[advanced] [=] default by value; mix with &y; prefer explicit lists\n";

    // -------------------------------------------------------------------------
    // §专家：跨作用域安全 + 默认捕获陷阱
    // -------------------------------------------------------------------------
    auto f = make_good();
    assert(f() == 42);

    // 大对象：值捕获会拷贝——只读且不需要拥有时，当场用完可考虑 const& 参数，
    // 或只捕获必要字段 [id=obj.id]（见 init_capture）。
    std::vector<int> big{1, 2, 3, 4, 5};
    auto sum_copy = [big] {
        int s = 0;
        for (int v : big) {
            s += v;
        }
        return s;
    };
    assert(sum_copy() == 15);
    big.push_back(6);
    assert(sum_copy() == 15);  // 仍是旧副本

    // 不可拷贝对象不能写 [p]（p 为 unique_ptr），要用 init-capture move
    // （见 init_capture_cpp14 topic）。

    // ⚠️ [=] 在成员函数里仍会按「指针」捕获 this（不是拷贝对象）——见 capture_this。
    std::cout << "[expert] value capture owns data → safe to store/return callable\n";
    std::cout << "=== capture_by_value: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_value", run>;

}  // namespace
