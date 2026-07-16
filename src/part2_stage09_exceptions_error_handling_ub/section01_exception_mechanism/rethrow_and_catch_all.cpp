// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : rethrow_and_catch_all
// Topic id : part2/stage09/section01/rethrow_and_catch_all
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <exception>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section01/rethrow_and_catch_all";

void low_level() {
    throw std::runtime_error{"disk unavailable"};
}

void add_context(std::string& log) {
    try {
        low_level();
    } catch (...) {
        log += "context added;";
        throw;  // Bare throw preserves the currently handled exception object.
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::string log;
    std::exception_ptr saved;
    try {
        add_context(log);
    } catch (...) {
        saved = std::current_exception();
    }
    LEARN_EXPECT_EQ(checks, log, std::string{"context added;"});
    LEARN_EXPECT(checks, saved != nullptr);
    LEARN_EXPECT_THROWS(checks, std::runtime_error, std::rethrow_exception(saved));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section01/rethrow_and_catch_all", run>;

}  // namespace
