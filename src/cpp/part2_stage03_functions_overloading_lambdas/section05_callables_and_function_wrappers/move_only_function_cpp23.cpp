// Topic     : std::move_only_function（C++23）
// Doc       : 第2部分-阶段3 · 步骤 7.4
// cppreference: https://en.cppreference.com/cpp/utility/functional/move_only_function
//
// 要点: 可存只移动可调用（捕获 unique_ptr 的 lambda）；不可拷贝；
//       支持 const/&&/noexcept 限定；空调用是 UB（不像 function 抛异常）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [move_only_function_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：装只移动 lambda
    // -------------------------------------------------------------------------
    auto p = std::make_unique<int>(42);
    std::move_only_function<int()> task = [p = std::move(p)] { return *p; };
    assert(task() == 42);
    std::cout << "[intro] move_only_function holds move-only lambda\n";

    // -------------------------------------------------------------------------
    // §进阶：只移动语义
    // -------------------------------------------------------------------------
    auto task2 = std::move(task);
    assert(task2() == 42);
    // auto task3 = task2; // ❌ 不可拷贝
    // 移动后源对象通常为空；调用空 move_only_function 是 UB——不要调用 task。

    // std::function 做不到：
    // std::function<int()> bad = [q = std::make_unique<int>(1)] { return *q; }; // ❌

    std::move_only_function<int(int)> add = [](int x) { return x + 1; };
    assert(add(5) == 6);
    std::cout << "[advanced] move-only; cannot copy; function rejects move-only targets\n";

    // -------------------------------------------------------------------------
    // §专家：限定符与空调用
    // -------------------------------------------------------------------------
    // 签名可带 const / && / noexcept，精确约束如何调用：
    std::move_only_function<int() const> ro = [] { return 7; };
    assert(ro() == 7);

    std::move_only_function<int() &&> once = [] { return 8; };
    assert(std::move(once)() == 8);

    // ⚠️ 空对象调用 = UB。用前保证非空；没有 target() RTTI API（比 function 更瘦）。
    std::move_only_function<void()> maybe;
    assert(!maybe);  // 可布尔检测
    maybe = [] {};
    assert(static_cast<bool>(maybe));
    maybe();  // 非空，OK

    std::cout << "[expert] cv/ref/noexcept on signature; empty call is UB — check first\n";
    std::cout << "=== move_only_function_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/move_only_function_cpp23", run>;

}  // namespace
