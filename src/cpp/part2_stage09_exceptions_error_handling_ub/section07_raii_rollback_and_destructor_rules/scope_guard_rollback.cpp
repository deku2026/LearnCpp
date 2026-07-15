// Topic    : RAII scope guard 回滚 / 事务式提交（dismiss）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 9.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : scope_guard_rollback
// Topic id : part2/stage09/section07/scope_guard_rollback
// Refs     : https://en.cppreference.com/w/cpp/language/raii
//            https://en.cppreference.com/w/cpp/experimental/scope_exit
//            C++ Core Guidelines E.19

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

class ScopeGuard {
    std::function<void()> rollback_;
    bool active_ = true;

public:
    explicit ScopeGuard(std::function<void()> f) : rollback_(std::move(f)) {}
    ~ScopeGuard() {
        if (active_ && rollback_) {
            rollback_();
        }
    }
    void dismiss() noexcept { active_ = false; }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

struct Ledger {
    std::vector<std::string> steps;
    void add(std::string s) { steps.push_back(std::move(s)); }
    void undo_last() {
        if (!steps.empty()) {
            steps.pop_back();
        }
    }
};

void transaction(Ledger& led, bool fail_at_b) {
    led.add("A");
    ScopeGuard undoA([&] { led.undo_last(); });

    led.add("B");
    ScopeGuard undoB([&] { led.undo_last(); });
    if (fail_at_b) {
        throw std::runtime_error("B failed");
    }

    led.add("C");
    ScopeGuard undoC([&] { led.undo_last(); });

    // 全部成功 → 提交
    undoA.dismiss();
    undoB.dismiss();
    undoC.dismiss();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [scope_guard_rollback] 主干：失败自动回滚 ===\n";
    {
        Ledger led;
        try {
            transaction(led, /*fail_at_b=*/true);
            assert(false);
        } catch (const std::runtime_error& e) {
            std::cout << "caught: " << e.what() << '\n';
        }
        // A、B 已加入但应被 guard 逆序撤销 → 空
        assert(led.steps.empty());
        std::cout << "after fail, steps empty (rolled back)\n";
    }

    std::cout << "=== 对抗：成功路径 dismiss 提交 ===\n";
    {
        Ledger led;
        transaction(led, /*fail_at_b=*/false);
        assert(led.steps.size() == 3);
        assert(led.steps[0] == "A" && led.steps[2] == "C");
        std::cout << "commit keeps A,B,C\n";
    }

    std::cout << "=== 专节：标准化方向 ===\n";
    // GSL gsl::finally / Boost.ScopeExit / experimental::scope_exit|fail|success
    // scope_fail：仅异常路径执行 —— 回滚理想工具
    std::cout << "pattern: acquire side-effect -> guard undo -> dismiss on success\n";

    std::cout << "[scope_guard_rollback] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section07/scope_guard_rollback", run>;

}  // namespace
