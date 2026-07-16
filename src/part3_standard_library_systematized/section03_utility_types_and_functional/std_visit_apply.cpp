// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : std_visit_apply
// Topic id : part3/section03/std_visit_apply
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part3/section03/std_visit_apply";

template <class... Callables>
struct Overload : Callables... {
    using Callables::operator()...;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::variant<int, std::string> value{std::string{"abc"}};
    const auto length = std::visit(Overload{
                                       [](int number) { return static_cast<std::size_t>(number); },
                                       [](const std::string& text) { return text.size(); },
                                   },
                                   value);
    LEARN_EXPECT_EQ(checks, length, 3U);

    const auto arguments = std::tuple{2, 3, 4};
    const int product = std::apply([](int first, int second, int third) { return first * second * third; }, arguments);
    LEARN_EXPECT_EQ(checks, product, 24);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/std_visit_apply", run>;

}  // namespace
