// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : bind_front_back_and_invoke_invoke_r_cpp23
// Topic id : part3/section03/bind_front_back_and_invoke_invoke_r_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section03/bind_front_back_and_invoke_invoke_r_cpp23";

int combine(int first, int second, int third) {
    return first * 100 + second * 10 + third;
}

struct Counter {
    int value;
    int add(int amount) const { return value + amount; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto front = std::bind_front(combine, 1, 2);
    LEARN_EXPECT_EQ(checks, front(3), 123);
#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    const auto back = std::bind_back(combine, 2, 3);
    LEARN_EXPECT_EQ(checks, back(1), 123);
#endif
    const Counter counter{7};
    LEARN_EXPECT_EQ(checks, std::invoke(&Counter::add, counter, 5), 12);
#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    LEARN_EXPECT_EQ(checks, std::invoke_r<long>(&Counter::add, counter, 5), 12L);
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part3/section03/bind_front_back_and_invoke_invoke_r_cpp23", run>;

}  // namespace
