// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : tuple_and_pair_overview
// Topic id : part3/section03/tuple_and_pair_overview
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part3/section03/tuple_and_pair_overview";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::pair<std::string, int> entry{"Ada", 10};
    auto tuple = std::tuple_cat(std::tuple{entry.first}, std::tuple{entry.second, true});
    static_assert(std::tuple_size_v<decltype(tuple)> == 3);
    LEARN_EXPECT_EQ(checks, std::get<0>(tuple), std::string{"Ada"});
    LEARN_EXPECT_EQ(checks, std::get<int>(tuple), 10);
    std::string name;
    int score{};
    std::tie(name, score) = entry;
    LEARN_EXPECT_EQ(checks, name, std::string{"Ada"});
    LEARN_EXPECT_EQ(checks, score, 10);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/tuple_and_pair_overview", run>;

}  // namespace
