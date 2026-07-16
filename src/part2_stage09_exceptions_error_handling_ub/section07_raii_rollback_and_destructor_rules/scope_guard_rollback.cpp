// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : scope_guard_rollback
// Topic id : part2/stage09/section07/scope_guard_rollback
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <functional>
#include <string_view>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section07/scope_guard_rollback";

class ScopeExit {
public:
    explicit ScopeExit(std::function<void()> action) : action_(std::move(action)) {}
    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
    ~ScopeExit() noexcept {
        if (active_) {
            action_();
        }
    }
    void release() noexcept { active_ = false; }

private:
    std::function<void()> action_;
    bool active_{true};
};

bool append_transaction(std::vector<int>& values, int value, bool commit) {
    const auto old_size = values.size();
    ScopeExit rollback{[&] { values.resize(old_size); }};
    values.push_back(value);
    if (!commit) {
        return false;
    }
    rollback.release();
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1};
    LEARN_EXPECT(checks, !append_transaction(values, 2, false));
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1}));
    LEARN_EXPECT(checks, append_transaction(values, 3, true));
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 3}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section07/scope_guard_rollback", run>;

}  // namespace
