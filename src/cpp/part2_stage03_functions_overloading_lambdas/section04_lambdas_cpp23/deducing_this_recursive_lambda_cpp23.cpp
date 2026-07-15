// Topic     : deducing this 递归 lambda（C++23, P0847）
// Doc       : 第2部分-阶段3 · 步骤 6.3
// cppreference: https://en.cppreference.com/cpp/language/member_functions
//               https://en.cppreference.com/cpp/language/lambda
//
// 要点: this auto self 为显式对象参数，self 即闭包本身 → 可递归；
//       相对 std::function 递归无类型擦除开销；阶段 4 系统讲 deducing this。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [deducing_this_recursive_lambda_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：斐波那契
    // -------------------------------------------------------------------------
    auto fib = [](this auto self, int n) -> int { return n < 2 ? n : self(n - 1) + self(n - 2); };
    assert(fib(0) == 0);
    assert(fib(1) == 1);
    assert(fib(10) == 55);
    std::cout << "[intro] this auto self enables direct recursion; fib(10)=55\n";

    // -------------------------------------------------------------------------
    // §进阶：阶乘 + 与 std::function 对照
    // -------------------------------------------------------------------------
    auto fact = [](this const auto& self, unsigned n) -> unsigned { return n <= 1 ? 1u : n * self(n - 1); };
    assert(fact(5) == 120);

    // 旧法：std::function 可递归但有类型擦除/可能堆分配
    std::function<int(int)> fib_sf = [&](int n) -> int { return n < 2 ? n : fib_sf(n - 1) + fib_sf(n - 2); };
    assert(fib_sf(10) == 55);
    std::cout << "[advanced] same result via std::function; deducing this is leaner\n";

    // -------------------------------------------------------------------------
    // §专家：显式对象参数语义
    // -------------------------------------------------------------------------
    // self 的类型/值类别由调用方式推导（this auto / this auto& / this const auto& ...）。
    // 泛型递归：
    auto sum_to = [](this auto self, int n, int acc = 0) -> int { return n <= 0 ? acc : self(n - 1, acc + n); };
    assert(sum_to(5) == 15);

    // 仍不能在「尚未声明完成的名字」上直接写 fib(n) 而不通过 self——self 就是出口。
    // 完整 deducing this（消除 const/&/&& 四份重载）见阶段 4。
    std::cout << "[expert] explicit object parameter binds to the closure object\n";
    std::cout << "=== deducing_this_recursive_lambda_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/deducing_this_recursive_lambda_cpp23", run>;

}  // namespace
