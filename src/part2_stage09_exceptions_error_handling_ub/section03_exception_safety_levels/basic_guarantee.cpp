// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : basic_guarantee
// Topic id : part2/stage09/section03/basic_guarantee
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section03/basic_guarantee";

struct Ledger {
    std::vector<int> entries;
    int total{};

    [[nodiscard]] bool invariant() const {
        int sum{};
        for (const int value : entries) {
            sum += value;
        }
        return sum == total;
    }
};

void append_then_report_failure(Ledger& ledger, int value) {
    ledger.entries.push_back(value);
    ledger.total += value;
    throw std::runtime_error{"remote acknowledgement failed"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Ledger ledger{{2, 3}, 5};
    // Capacity is not part of the lesson. Reserve the known final size so the
    // example isolates the downstream failure after the state change.
    ledger.entries.reserve(3);
    LEARN_EXPECT_THROWS(checks, std::runtime_error, append_then_report_failure(ledger, 4));
    LEARN_EXPECT(checks, ledger.invariant());
    LEARN_EXPECT_EQ(checks, ledger.total, 9);  // Valid but observably changed: basic, not strong.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section03/basic_guarantee", run>;

}  // namespace
