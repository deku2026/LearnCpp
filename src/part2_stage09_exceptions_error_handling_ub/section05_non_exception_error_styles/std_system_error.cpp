// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_system_error
// Topic id : part2/stage09/section05/std_system_error
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section05/std_system_error";

void open_protected_resource() {
    throw std::system_error{std::make_error_code(std::errc::permission_denied), "open settings"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    bool inspected = false;
    try {
        open_protected_resource();
    } catch (const std::system_error& error) {
        inspected = error.code() == std::errc::permission_denied && std::string{error.what()}.contains("open settings");
    }
    LEARN_EXPECT(checks, inspected);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section05/std_system_error", run>;

}  // namespace
