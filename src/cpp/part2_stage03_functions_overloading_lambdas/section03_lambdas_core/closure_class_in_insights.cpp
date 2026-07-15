// Topic     : 闭包类本质（C++ Insights 视角）
// Doc       : 第2部分-阶段3 · 步骤 5.2 / 5.9
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: lambda = 唯一未命名闭包类型的 prvalue；捕获 → 数据成员；体 → operator()；
//       默认 const 调用运算符；无捕获可隐式转函数指针；手写仿函数对照。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace {

// 手写「等价仿函数」，对照闭包类
struct AddN {
    int n;
    explicit AddN(int n_) : n(n_) {}
    int operator()(int x) const { return x + n; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [closure_class_in_insights] ===\n";

    // -------------------------------------------------------------------------
    // §入门：lambda 就是带 operator() 的对象
    // -------------------------------------------------------------------------
    int n = 10;
    auto add_n = [n](int x) { return x + n; };
    assert(add_n(5) == 15);

    AddN hand{10};
    assert(hand(5) == 15);
    // 把本文件的 lambda 贴进 https://cppinsights.io 可看到类似 AddN 的生成类
    std::cout << "[intro] lambda object ≈ compiler-written functor with call operator\n";

    // -------------------------------------------------------------------------
    // §进阶：类型唯一、const、成员
    // -------------------------------------------------------------------------
    auto add_n2 = [n](int x) { return x + n; };
    static_assert(!std::is_same_v<decltype(add_n), decltype(add_n2)>);  // 每个 lambda 表达式唯一类型

    // 默认 operator() const → 不能改值捕获（除非 mutable）
    // 闭包不可默认构造（有捕获时）、不可拷贝赋值（通常）
    auto copy = add_n;
    assert(copy(1) == 11);

    std::cout << "[advanced] each lambda expression has a unique closure type\n";

    // -------------------------------------------------------------------------
    // §专家：无捕获 → 函数指针；有捕获则否
    // -------------------------------------------------------------------------
    int arr[] = {3, 1, 2};
    std::qsort(arr, 3, sizeof(int), [](const void* a, const void* b) {
        const int ia = *static_cast<const int*>(a);
        const int ib = *static_cast<const int*>(b);
        return (ia > ib) - (ia < ib);
    });
    assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3);

    using Cmp = int (*)(const void*, const void*);
    Cmp fp = [](const void*, const void*) { return 0; };  // 无捕获 OK
    assert(fp(nullptr, nullptr) == 0);
    // int cap = 1;
    // Cmp bad = [cap](const void*, const void*) { return cap; }; // ❌

    // 闭包类：operator() 不 virtual；C++14 起无默认 ctor（无捕获在 C++20 起可默认构造等细节见标准演进）
    std::cout << "[expert] captureless lambda converts to function pointer; open cppinsights.io\n";
    std::cout << "=== closure_class_in_insights: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/closure_class_in_insights", run>;

}  // namespace
