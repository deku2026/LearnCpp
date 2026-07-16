// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : function_versus_move_only_function_cpp23
// Topic id : part3/section03/function_versus_move_only_function_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <functional>
#include <memory>
#include <string_view>
#include <type_traits>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section03/function_versus_move_only_function_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::function<int(int)> copyable = [offset = 2](int value) { return value + offset; };
    const auto copy = copyable;
    LEARN_EXPECT_EQ(checks, copy(3), 5);
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    std::move_only_function<int(int)> move_only = [owner = std::make_unique<int>(4)](int value) {
        return value + *owner;
    };
    static_assert(!std::copy_constructible<decltype(move_only)>);
    LEARN_EXPECT_EQ(checks, move_only(3), 7);
#else
    LEARN_EXPECT(checks, static_cast<bool>(copyable));
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part3/section03/function_versus_move_only_function_cpp23", run>;

}  // namespace
