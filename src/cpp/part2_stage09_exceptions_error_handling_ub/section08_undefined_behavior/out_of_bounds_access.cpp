// Topic    : 越界访问 UB —— 安全对照（at / span / 范围检查）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : out_of_bounds_access
// Topic id : part2/stage09/section08/out_of_bounds_access
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            https://en.cppreference.com/w/cpp/container/vector/at
//            ASan

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <span>
#include <stdexcept>
#include <vector>

namespace {

int sum_span(std::span<const int> s) {
    int total = 0;
    for (int x : s) {
        total += x;
    }
    return total;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [out_of_bounds_access] 主干：.at() 抛异常（非 UB）===\n";
    {
        std::vector<int> v{10, 20, 30};
        assert(v.at(1) == 20);
        try {
            (void)v.at(3);  // size=3，合法下标 0..2
            assert(false);
        } catch (const std::out_of_range& e) {
            std::cout << "v.at(3): " << e.what() << '\n';
        }
    }

    std::cout << "=== 对抗：span / 范围 for 避免手写下标 ===\n";
    {
        int arr[] = {1, 2, 3, 4};
        assert(sum_span(arr) == 10);
        std::cout << "span sum=" << sum_span(arr) << '\n';
    }

    std::cout << "=== 专节：UB 形态与 Sanitizer（不触发）===\n";
    // 危险（勿运行）：
    //   int a[3]{}; int x = a[3];      // 越界读 UB
    //   std::vector<int> v(3); v[3]=1; // operator[] 不检查 → UB
    // 护栏：
    //   clang++ -fsanitize=address -g ...
    //   clang-cl /fsanitize=address ...
    std::cout << "prefer at()/span/range-for; ASan catches OOB in tests\n";
    std::cout << "this topic never executes OOB on purpose\n";

    std::cout << "[out_of_bounds_access] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/out_of_bounds_access", run>;

}  // namespace
