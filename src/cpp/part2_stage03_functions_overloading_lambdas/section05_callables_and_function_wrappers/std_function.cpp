// Topic     : std::function 类型擦除可调用包装
// Doc       : 第2部分-阶段3 · 步骤 7.3
// cppreference: https://en.cppreference.com/cpp/utility/functional/function
//
// 要点: 统一存储签名兼容的可调用；可拷贝；空则抛 bad_function_call；
//       有类型擦除开销；只移动可调用装不进（见 move_only_function）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

int free_double(int x) {
    return x * 2;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_function] ===\n";

    // -------------------------------------------------------------------------
    // §入门：装 lambda / 函数指针
    // -------------------------------------------------------------------------
    std::function<int(int)> f = [](int x) { return x + 1; };
    assert(f(10) == 11);

    f = free_double;
    assert(f(10) == 20);

    int base = 100;
    f = [base](int x) { return x + base; };
    assert(f(10) == 110);
    std::cout << "[intro] std::function erases callable type under a signature\n";

    // -------------------------------------------------------------------------
    // §进阶：容器化回调列表
    // -------------------------------------------------------------------------
    std::vector<std::function<int(int)>> handlers;
    handlers.push_back([](int x) { return x * 2; });
    handlers.push_back([](int x) { return x - 1; });
    assert(handlers[0](5) == 10);
    assert(handlers[1](5) == 4);

    // 布尔上下文：是否为空
    std::function<void()> empty;
    assert(!empty);
    empty = [] {};
    assert(static_cast<bool>(empty));
    std::cout << "[advanced] vector of callbacks; empty check via operator bool\n";

    // -------------------------------------------------------------------------
    // §专家：空调用异常、代价、可拷贝约束
    // -------------------------------------------------------------------------
    std::function<int()> vacant;
    bool threw = false;
    try {
        (void)vacant();
    } catch (const std::bad_function_call&) {
        threw = true;
    }
    assert(threw);

    // 目标可拷贝：function 自身可拷贝
    std::function<int(int)> g = f;
    assert(g(1) == 101);

    // 大闭包可能触发 SBO 失败 → 堆分配（实现定义）
    // 只移动 lambda 无法赋给 std::function —— 见 move_only_function topic。
    std::cout << "[expert] empty call throws bad_function_call; requires CopyConstructible target\n";
    std::cout << "=== std_function: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_function", run>;

}  // namespace
