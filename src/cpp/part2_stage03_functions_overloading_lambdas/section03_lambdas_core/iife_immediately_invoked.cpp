// Topic     : 立即调用 lambda（IIFE）
// Doc       : 第2部分-阶段3 · 步骤 5.8
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 定义后立刻 () 调用；适合用复杂逻辑初始化 const；
//       限制作用域、避免中间可变变量泄漏。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool is_prime(int n) {
    if (n < 2) {
        return false;
    }
    for (int d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iife_immediately_invoked] ===\n";

    // -------------------------------------------------------------------------
    // §入门：定义即调用
    // -------------------------------------------------------------------------
    [[maybe_unused]] const int answer = [] { return 40 + 2; }();
    assert(answer == 42);
    std::cout << "[intro] []{ ... }() runs immediately\n";

    // -------------------------------------------------------------------------
    // §进阶：初始化 const 容器
    // -------------------------------------------------------------------------
    const std::vector<int> primes = [] {
        std::vector<int> v;
        for (int n = 2; n < 20; ++n) {
            if (is_prime(n)) {
                v.push_back(n);
            }
        }
        return v;
    }();
    assert(primes.size() == 8);
    assert(primes.front() == 2 && primes.back() == 19);
    // primes.push_back(1); // ❌ const
    std::cout << "[advanced] IIFE builds const vector of primes < 20, size=" << primes.size() << '\n';

    // -------------------------------------------------------------------------
    // §专家：捕获、分支初始化、与语句表达式对比
    // -------------------------------------------------------------------------
    const std::string mode = "prod";
    const std::string banner = [&] {
        if (mode == "prod") {
            return std::string{"[PROD]"};
        }
        return std::string{"[DEV]"};
    }();
    assert(banner == "[PROD]");

    // 比「先非 const 填充再当 const 用」更能表达不可变性意图。
    // 注意：IIFE 的临时闭包在 ; 处结束寿命；不要返回指向其捕获局部的引用。
    int base = 3;
    [[maybe_unused]] const int scaled = [base] {
        int r = 1;
        for (int i = 0; i < 3; ++i) {
            r *= base;
        }
        return r;
    }();
    assert(scaled == 27);

    std::cout << "[expert] IIFE scopes helper logic; keep returned values owned/copied\n";
    std::cout << "=== iife_immediately_invoked: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/iife_immediately_invoked", run>;

}  // namespace
