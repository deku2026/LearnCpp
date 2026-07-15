// Topic     : 仿函数（functor）基础
// Doc       : 第2部分-阶段3 · 步骤 7.1
// cppreference: https://en.cppreference.com/cpp/language/operators
//
// 要点: 带 operator() 的类 = 可携带状态的可调用；lambda 是编译器写的仿函数；
//       STL 算法接受函数对象作为策略。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Adder {
    int base;
    int operator()(int x) const { return x + base; }
};

struct Accumulator {
    int sum = 0;
    int operator()(int x) {
        sum += x;
        return sum;
    }
};

struct LessBySize {
    bool operator()(const std::string& a, const std::string& b) const { return a.size() < b.size(); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [functor_basics] ===\n";

    // -------------------------------------------------------------------------
    // §入门：手写 operator()
    // -------------------------------------------------------------------------
    Adder add5{5};
    assert(add5(10) == 15);
    // 等价 lambda:
    auto add5_lam = [base = 5](int x) { return x + base; };
    assert(add5_lam(10) == 15);
    std::cout << "[intro] functor carries state; lambda is shorthand\n";

    // -------------------------------------------------------------------------
    // §进阶：可变状态 + 算法策略
    // -------------------------------------------------------------------------
    Accumulator acc;
    assert(acc(3) == 3);
    assert(acc(4) == 7);
    assert(acc.sum == 7);

    std::vector<std::string> words{"ccc", "a", "bb"};
    std::sort(words.begin(), words.end(), LessBySize{});
    assert(words[0] == "a" && words[1] == "bb" && words[2] == "ccc");
    std::cout << "[advanced] stateful functor; custom comparator object\n";

    // -------------------------------------------------------------------------
    // §专家：重载 call op、const、透明比较预告
    // -------------------------------------------------------------------------
    struct Poly {
        int operator()(int x) const { return x; }
        double operator()(double x) const { return x * 2.0; }
    };
    Poly p;
    assert(p(3) == 3);
    assert(p(1.5) == 3.0);

    // 仿函数可有默认构造、可嵌套类型、可继承——lambda 闭包类型则更受限。
    // 现代代码：短逻辑用 lambda；要名字、要复用、要多 operator() 重载 → 写类。
    std::cout << "[expert] overload call ops; choose class vs lambda by reuse needs\n";
    std::cout << "=== functor_basics: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/functor_basics", run>;

}  // namespace
