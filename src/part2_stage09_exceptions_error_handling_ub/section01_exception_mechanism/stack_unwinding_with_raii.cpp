// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : stack_unwinding_with_raii
// Topic id : part2/stage09/section01/stack_unwinding_with_raii
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section01/stack_unwinding_with_raii";

struct TraceScope {
    TraceScope(std::vector<std::string>& destination, std::string scope_label)
        : output(destination), label(std::move(scope_label)) {
        output.push_back(this->label + "+");
    }
    ~TraceScope() { output.push_back(label + "-"); }

    std::vector<std::string>& output;
    std::string label;
};

void fail_after_acquiring(std::vector<std::string>& events) {
    TraceScope outer{events, "outer"};
    TraceScope inner{events, "inner"};
    throw std::runtime_error{"rollback"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<std::string> events;
    try {
        fail_after_acquiring(events);
    } catch (const std::runtime_error&) {
        events.push_back("caught");
    }
    const std::vector<std::string> expected{"outer+", "inner+", "inner-", "outer-", "caught"};
    LEARN_EXPECT_EQ(checks, events, expected);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section01/stack_unwinding_with_raii", run>;

}  // namespace
