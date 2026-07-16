// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : dangling_reference
// Topic id : part2/stage09/section08/dangling_reference
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <functional>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/dangling_reference";

std::string make_name() {
    return "Ada";
}

std::reference_wrapper<const std::string> borrow_name(const std::string& owner) {
    return owner;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string owned = make_name();
    const auto borrowed = borrow_name(owned);
    LEARN_EXPECT_EQ(checks, borrowed.get(), std::string{"Ada"});
#if 0
    const std::string& dangling = make_name();  // A returned temporary cannot back a lasting borrow.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/dangling_reference", run>;

}  // namespace
