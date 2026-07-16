// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : std_exception_hierarchy
// Topic id : part2/stage09/section01/std_exception_hierarchy
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <concepts>
#include <stdexcept>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section01/std_exception_hierarchy";

static_assert(std::derived_from<std::logic_error, std::exception>);
static_assert(std::derived_from<std::invalid_argument, std::logic_error>);
static_assert(std::derived_from<std::system_error, std::runtime_error>);

void reject_index(int index) {
    if (index < 0) {
        throw std::invalid_argument{"negative index"};
    }
    throw std::out_of_range{"index too large"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    for (const int index : {-1, 99}) {
        try {
            reject_index(index);
        } catch (const std::invalid_argument& error) {  // Derived handlers precede their bases.
            LEARN_EXPECT(checks, std::string_view{error.what()}.contains("negative"));
        } catch (const std::logic_error& error) {
            LEARN_EXPECT(checks, std::string_view{error.what()}.contains("large"));
        }
    }
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section01/std_exception_hierarchy", run>;

}  // namespace
