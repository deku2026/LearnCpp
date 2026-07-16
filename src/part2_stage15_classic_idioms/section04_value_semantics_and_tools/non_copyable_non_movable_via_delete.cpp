// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : non_copyable_non_movable_via_delete
// Topic id : part2/stage15/section04/non_copyable_non_movable_via_delete
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section04/non_copyable_non_movable_via_delete";

class ProcessLock {
public:
    ProcessLock() = default;
    ProcessLock(const ProcessLock&) = delete;
    ProcessLock& operator=(const ProcessLock&) = delete;
    ProcessLock(ProcessLock&&) = delete;
    ProcessLock& operator=(ProcessLock&&) = delete;
    void acquire() noexcept { held_ = true; }
    bool held() const noexcept { return held_; }

private:
    bool held_{};
};

static_assert(!std::copy_constructible<ProcessLock>);
static_assert(!std::move_constructible<ProcessLock>);
static_assert(!std::is_copy_assignable_v<ProcessLock>);
static_assert(!std::is_move_assignable_v<ProcessLock>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    ProcessLock lock;
    lock.acquire();
    LEARN_EXPECT(checks, lock.held());
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage15/section04/non_copyable_non_movable_via_delete", run>;

}  // namespace
