// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : erase_remove_idiom
// Topic id : part2/stage15/section01/erase_remove_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section01/erase_remove_idiom";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> classic{1, 2, 3, 2, 4};
    const auto logical_end = std::remove(classic.begin(), classic.end(), 2);
    LEARN_EXPECT_EQ(checks, std::distance(classic.begin(), logical_end), 3);
    classic.erase(logical_end, classic.end());
    LEARN_EXPECT_EQ(checks, classic, std::vector<int>({1, 3, 4}));

    std::vector<int> modern{1, 2, 3, 4, 5};
    const auto removed = std::erase_if(modern, [](int value) { return value % 2 == 0; });
    LEARN_EXPECT_EQ(checks, removed, 2U);
    LEARN_EXPECT_EQ(checks, modern, std::vector<int>({1, 3, 5}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section01/erase_remove_idiom", run>;

}  // namespace
