// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : reference_wrapper_and_forward_like_cpp23
// Topic id : part3/section03/reference_wrapper_and_forward_like_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <functional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section03/reference_wrapper_and_forward_like_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int first = 1;
    int second = 2;
    std::vector<std::reference_wrapper<int>> references{first, second};
    references[0].get() = 7;
    LEARN_EXPECT_EQ(checks, first, 7);
#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    int value = 9;
    static_assert(std::same_as<decltype(std::forward_like<const int&>(value)), const int&>);
    static_assert(std::same_as<decltype(std::forward_like<int&&>(value)), int&&>);
    LEARN_EXPECT_EQ(checks, std::forward_like<const int&>(value), 9);
#else
    LEARN_EXPECT_EQ(checks, second, 2);
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part3/section03/reference_wrapper_and_forward_like_cpp23", run>;

}  // namespace
