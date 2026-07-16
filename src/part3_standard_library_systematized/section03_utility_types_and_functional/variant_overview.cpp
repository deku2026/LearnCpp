// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : variant_overview
// Topic id : part3/section03/variant_overview
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <string>
#include <string_view>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part3/section03/variant_overview";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::variant<std::monostate, int, std::string> value;
    LEARN_EXPECT(checks, std::holds_alternative<std::monostate>(value));
    value = 7;
    LEARN_EXPECT_EQ(checks, std::get<int>(value), 7);
    LEARN_EXPECT(checks, std::get_if<std::string>(&value) == nullptr);
    value.emplace<std::string>("ready");
    LEARN_EXPECT_EQ(checks, std::get<2>(value), std::string{"ready"});
    LEARN_EXPECT_THROWS(checks, std::bad_variant_access, std::get<int>(value));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/variant_overview", run>;

}  // namespace
