// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : any_overview
// Topic id : part3/section03/any_overview
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <any>
#include <concepts>
#include <string>
#include <string_view>
#include <typeinfo>

namespace {

constexpr std::string_view kTopic = "part3/section03/any_overview";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::any value = 42;
    LEARN_EXPECT(checks, value.has_value());
    LEARN_EXPECT(checks, value.type() == typeid(int));
    LEARN_EXPECT_EQ(checks, std::any_cast<int>(value), 42);
    LEARN_EXPECT(checks, std::any_cast<std::string>(&value) == nullptr);
    LEARN_EXPECT_THROWS(checks, std::bad_any_cast, std::any_cast<std::string>(value));
    value.emplace<std::string>(3, 'x');
    LEARN_EXPECT_EQ(checks, std::any_cast<const std::string&>(value), std::string{"xxx"});
    value.reset();
    LEARN_EXPECT(checks, !value.has_value());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/any_overview", run>;

}  // namespace
