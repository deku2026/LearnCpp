// Topic    : std::unreachable：标记不可达（走错即 UB）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 8.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_unreachable_cpp23
// Topic id : part2/stage09/section06/std_unreachable_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/unreachable
//            P0627
//            ISO [utility.unreachable]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <utility>

namespace {

// 仅在 x∈{0,1,2} 时调用 —— default 用 unreachable 告知优化器
int categorize(int x) {
    switch (x) {
        case 0:
            return 10;
        case 1:
            return 20;
        case 2:
            return 30;
        default:
            // ⚠️ 若实际执行到这里 → UB
            // 教学：下面用 assert 挡住非法输入，保证 topic 不触发 UB。
            // 真正的 unreachable 写法是：std::unreachable();
            assert(false && "categorize precondition violated; would be unreachable");
            return -1;
    }
}

// 安全对照：非法输入用错误处理，而不是 unreachable
enum class CatErr { BadInput };

std::pair<int, bool> categorize_safe(int x) {
    switch (x) {
        case 0:
            return {10, true};
        case 1:
            return {20, true};
        case 2:
            return {30, true};
        default:
            return {-1, false};
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_unreachable_cpp23] 主干：合法输入路径 ===\n";
    {
        assert(categorize(0) == 10);
        assert(categorize(1) == 20);
        assert(categorize(2) == 30);
        std::cout << "categorize 0/1/2 -> 10/20/30\n";
    }

    std::cout << "=== 对抗：安全 API 处理非法输入（非 UB）===\n";
    {
        auto [v, ok] = categorize_safe(5);
        assert(!ok && v == -1);
        std::cout << "safe path rejects 5 without UB\n";
    }

    std::cout << "=== 专节：unreachable 纪律（不故意触发）===\n";
    // std::unreachable() 与 [[assume]] 同源：你对编译器的承诺。
    // 走错 → UB（可能返回垃圾、乱跳）。
    // 只在 100% 逻辑不可达时用；能错误处理的地方别用。
    // 正确用法示意（仅注释）：
    //   default: std::unreachable();
    // 错误用法：把用户输入直接丢进带 unreachable 的 default。
    std::cout << "unreachable = optimizer contract; wrong path = UB\n";
    std::cout << "ASan/UBSan may not always catch it — discipline first\n";
    // 引用函数以免被优化掉认知
    (void)&std::unreachable;

    std::cout << "[std_unreachable_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/std_unreachable_cpp23", run>;

}  // namespace
