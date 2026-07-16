// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : structured_bindings
// Topic id : part3/section03/structured_bindings
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <array>
#include <concepts>
#include <map>
#include <string>
#include <string_view>
#include <tuple>

namespace {

constexpr std::string_view kTopic = "part3/section03/structured_bindings";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::array values{1, 2};
    auto& [first, second] = values;
    first = 7;
    LEARN_EXPECT_EQ(checks, values[0], 7);
    LEARN_EXPECT_EQ(checks, second, 2);

    const std::map<std::string, int> scores{{"Ada", 10}};
    for (const auto& [name, score] : scores) {
        LEARN_EXPECT_EQ(checks, name, std::string{"Ada"});
        LEARN_EXPECT_EQ(checks, score, 10);
    }
    const auto [copy_first, copy_second] = values;
    LEARN_EXPECT_EQ(checks, copy_first + copy_second, 9);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/structured_bindings", run>;

}  // namespace
