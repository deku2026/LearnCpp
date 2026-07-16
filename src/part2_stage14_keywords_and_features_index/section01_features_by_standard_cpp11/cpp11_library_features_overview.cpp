// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section01_features_by_standard_cpp11
// Item     : cpp11_library_features_overview
// Topic id : part2/stage14/section01/cpp11_library_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <chrono>
#include <memory>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section01/cpp11_library_features_overview";

constexpr std::array<std::string_view, 10> features{
    "array",  "tuple",     "type_traits", "smart pointers", "unordered containers",
    "chrono", "threading", "random",      "regex",          "function/bind",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::array values{1, 2, 3};
    const auto record = std::make_tuple("Ada", 10);
    auto owner = std::make_shared<int>(42);
    const std::unordered_map<int, int> squares{{2, 4}, {3, 9}};
    static_assert(std::is_integral_v<decltype(values)::value_type>);
    LEARN_EXPECT_EQ(checks, std::get<1>(record), 10);
    LEARN_EXPECT_EQ(checks, owner.use_count(), 1L);
    LEARN_EXPECT_EQ(checks, squares.at(3), 9);
    LEARN_EXPECT(checks, std::chrono::seconds{1} == std::chrono::milliseconds{1'000});
    LEARN_EXPECT_EQ(checks, features.size(), 10U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section01/cpp11_library_features_overview", run>;

}  // namespace
