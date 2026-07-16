// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_must_not_throw
// Topic id : part2/stage09/section07/destructor_must_not_throw
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <exception>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section07/destructor_must_not_throw";

struct SafeCleanup {
    explicit SafeCleanup(bool& error_observed) : observed(error_observed) {}
    ~SafeCleanup() noexcept {
        try {
            throw std::runtime_error{"simulated close failure"};
        } catch (...) {
            observed = true;  // Report or store the error; do not let it escape the destructor.
        }
    }
    bool& observed;
};

static_assert(std::is_nothrow_destructible_v<SafeCleanup>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    bool observed = false;
    {
        SafeCleanup cleanup{observed};
        (void)cleanup;
    }
    LEARN_EXPECT(checks, observed);
    LEARN_EXPECT_EQ(checks, std::uncaught_exceptions(), 0);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section07/destructor_must_not_throw", run>;

}  // namespace
