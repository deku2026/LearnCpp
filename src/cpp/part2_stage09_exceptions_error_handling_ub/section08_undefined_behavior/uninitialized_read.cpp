// Topic    : 未初始化读 UB —— 声明即初始化对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : uninitialized_read
// Topic id : part2/stage09/section08/uninitialized_read
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            https://en.cppreference.com/w/cpp/language/default_initialization
//            MSan / UBSan

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

namespace {

struct Point {
    int x = 0;  // 类内初始化：避免未初始化
    int y = 0;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [uninitialized_read] 主干：声明即初始化 ===\n";
    {
        int x = 0;  // ✅
        assert(x == 0);

        Point p;  // 成员已有默认值
        assert(p.x == 0 && p.y == 0);

        std::vector<int> v(3);  // 值初始化为 0
        assert(v[0] == 0 && v[2] == 0);
        std::cout << "value-init vector/Point ok\n";
    }

    std::cout << "=== 对抗：optional 表达“尚未赋值”===\n";
    {
        std::optional<int> maybe;
        assert(!maybe);
        maybe = 7;
        assert(*maybe == 7);
        std::cout << "optional distinguishes unset vs set\n";
    }

    std::cout << "=== 专节：UB 形态（不触发）===\n";
    // 危险（勿运行）：
    //   int x; use(x);           // 未初始化读 UB
    //   int a[10]; use(a[0]);    // 默认初始化的局部数组元素不确定
    // 工具：MemorySanitizer / 部分 UBSan；更可靠是编码纪律。
    std::cout << "always initialize; MSan finds many uninit reads\n";

    std::cout << "[uninitialized_read] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/uninitialized_read", run>;

}  // namespace
