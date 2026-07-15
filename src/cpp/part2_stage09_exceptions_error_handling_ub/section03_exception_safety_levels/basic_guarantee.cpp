// Topic    : Basic 基本异常保证：不泄漏，对象仍有效（状态可能变）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 3.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : basic_guarantee
// Topic id : part2/stage09/section03/basic_guarantee
// Refs     : https://en.cppreference.com/w/cpp/language/exceptions
//            C++ Core Guidelines E.6

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// 基本保证：抛异常后不泄漏资源，对象可析构/可再赋值，但中间状态未指定
class Accumulator {
    std::vector<std::string> items_;
    int total_len_ = 0;

public:
    int total_len() const noexcept { return total_len_; }
    std::size_t size() const noexcept { return items_.size(); }

    // 可能在 push 后、更新 total 前抛 —— 状态“有效但不一致”
    void add(std::string s, bool fail_after_push) {
        items_.push_back(std::move(s));  // 可能抛；若抛则 *this 未改（更强）
        if (fail_after_push) {
            // 已 push，但 total_len_ 未更新 → 不变量暂时破坏
            // 仍满足 basic：无泄漏、对象可析构
            throw std::runtime_error("fail after push");
        }
        total_len_ += static_cast<int>(items_.back().size());
    }

    // 修复不变量的“恢复路径”示例（真实代码应避免打破不变量）
    void recompute() noexcept {
        total_len_ = 0;
        for (const auto& s : items_) {
            total_len_ += static_cast<int>(s.size());
        }
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [basic_guarantee] 主干：抛后不泄漏、对象仍可用 ===\n";
    {
        Accumulator acc;
        acc.add("hi", /*fail_after_push=*/false);
        assert(acc.size() == 1 && acc.total_len() == 2);

        try {
            acc.add("there", /*fail_after_push=*/true);
            assert(false);
        } catch (const std::runtime_error& e) {
            std::cout << "caught: " << e.what() << '\n';
        }
        // basic：size 可能已变，total_len 可能过期 —— 但无泄漏、可继续用
        assert(acc.size() == 2);
        assert(acc.total_len() == 2);  // 未更新，不一致
        acc.recompute();
        assert(acc.total_len() == 2 + 5);
        std::cout << "after basic-failure + recompute, total_len=" << acc.total_len() << '\n';
    }

    std::cout << "=== 对抗：RAII 帮助达成至少 basic ===\n";
    {
        // 用 unique_ptr 管理堆：任何路径抛异常都自动释放 → 不泄漏
        auto p = std::make_unique<int[]>(1000);
        try {
            p[0] = 1;
            throw std::logic_error("abort");
        } catch (const std::logic_error&) {
            // p 即将析构
        }
        // 离开作用域后资源释放；basic 的“不泄漏”靠 RAII
        std::cout << "unique_ptr ensures no leak on exception (basic floor)\n";
    }

    std::cout << "=== 专节：目标层级 ===\n";
    // 最低要求：至少 basic（不泄漏、对象有效）
    // 关键操作：争取 strong
    // swap/移动/析构：no-throw
    std::cout << "floor=basic; prefer strong for mutating APIs\n";

    std::cout << "[basic_guarantee] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/basic_guarantee", run>;

}  // namespace
