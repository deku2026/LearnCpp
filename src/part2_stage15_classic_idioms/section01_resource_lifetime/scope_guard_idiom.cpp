// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : scope_guard_idiom
// Topic id : part2/stage15/section01/scope_guard_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <functional>
#include <string_view>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section01/scope_guard_idiom";

class ScopeExit {
public:
    explicit ScopeExit(std::function<void()> action) : action_(std::move(action)) {}
    ScopeExit(const ScopeExit&) = delete;
    ~ScopeExit() noexcept {
        if (active_) action_();
    }
    void release() noexcept { active_ = false; }

private:
    std::function<void()> action_;
    bool active_{true};
};

void append(std::vector<int>& values, bool commit) {
    const auto old_size = values.size();
    ScopeExit rollback{[&] { values.resize(old_size); }};
    values.push_back(7);
    if (commit) rollback.release();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1};
    append(values, false);
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1}));
    append(values, true);
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 7}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section01/scope_guard_idiom", run>;

}  // namespace
