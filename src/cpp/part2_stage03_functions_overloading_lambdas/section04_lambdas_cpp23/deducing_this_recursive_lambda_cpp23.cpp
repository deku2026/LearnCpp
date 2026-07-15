// Topic     : deducing this 递归 lambda（C++23, P0847）
// Doc       : 第2部分-阶段3 · 步骤 6.3 / 验收「静态 lambda + 递归 lambda」
// cppreference: https://en.cppreference.com/cpp/language/member_functions
//               https://en.cppreference.com/cpp/language/lambda
// 提案      : P0847R7
//
// 要点: this auto self 为显式对象参数，self 即闭包自身 → 可递归；
//       省掉 std::function 自引用的类型擦除开销；阶段 4 系统讲 deducing this。
//       对照：旧法 std::function 递归 / 经典 y_combinator。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

// 经典「把自己当第一参数」的 y_combinator 风格（C++14+），对照 deducing this
template <class F>
struct Y {
    F f;
    template <class... Args>
    decltype(auto) operator()(Args&&... args) const {
        return f(*this, std::forward<Args>(args)...);
    }
};
template <class F>
Y(F) -> Y<F>;

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
    assert(fact(0) == 1);

    // 旧方案：std::function 可递归但有类型擦除 / 可能堆分配
    std::function<int(int)> fib_sf = [&](int n) -> int { return n < 2 ? n : fib_sf(n - 1) + fib_sf(n - 2); };
    assert(fib_sf(10) == 55);
    std::cout << "[advanced] fact(5)=120; same fib via std::function (heavier)\n";

    // -------------------------------------------------------------------------
    // §专家：尾递归累加、y_combinator 对照、值类别
    // -------------------------------------------------------------------------
    // self 的类型/值类别由调用方式推导（this auto / this auto& / this const auto& …）
    auto sum_to = [](this auto self, int n, int acc = 0) -> int { return n <= 0 ? acc : self(n - 1, acc + n); };
    assert(sum_to(5) == 15);

    // 名称尚未绑定完毕时不能写 fib(n-1)，必须通过 self——self 就是「自己」
    // y_combinator 把「可调用自己」外置成包装类型；C++23 把对象参数显式化后不再需要
    auto fib_y = Y{[](auto self, int n) -> int { return n < 2 ? n : self(n - 1) + self(n - 2); }};
    assert(fib_y(10) == 55);

    // 带捕获的递归（计数调用）
    int calls = 0;
    auto fib_count = [&calls](this auto self, int n) -> int {
        ++calls;
        return n < 2 ? n : self(n - 1) + self(n - 2);
    };
    assert(fib_count(6) == 8);
    assert(calls > 6);

    // 扩展到成员函数的 deducing this（合并 const/&/&& 四份重载）见阶段 4
    static_assert(std::is_invocable_r_v<int, decltype(fib), int>);
    std::cout << "[expert] y_combinator parity; fib_count calls=" << calls << "; prefer this auto self (P0847)\n";
    std::cout << "=== deducing_this_recursive_lambda_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/deducing_this_recursive_lambda_cpp23", run>;

}  // namespace
