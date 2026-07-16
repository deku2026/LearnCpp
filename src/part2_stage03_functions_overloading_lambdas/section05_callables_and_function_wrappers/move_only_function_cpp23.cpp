// C++23 move_only_function can own a callable that cannot be copied.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : move_only_function_cpp23
// Topic id : part2/stage03/section05/move_only_function_cpp23
//
// Unlike an empty std::function, calling an empty move_only_function is UB;
// this example constructs it before every call and never invokes moved-from state.

#include "learn/example_support.hpp"

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    learn::ExampleChecks checks{"part2/stage03/section05/move_only_function_cpp23"};

    auto closure = [state = std::make_unique<int>(40)]() mutable { return ++*state; };
    static_assert(std::is_move_constructible_v<decltype(closure)>);
    static_assert(!std::is_copy_constructible_v<decltype(closure)>);

    std::move_only_function<int()> task{std::move(closure)};
    static_assert(std::is_move_constructible_v<decltype(task)>);
    static_assert(!std::is_copy_constructible_v<decltype(task)>);
    LEARN_EXPECT(checks, static_cast<bool>(task));
    LEARN_EXPECT_EQ(checks, task(), 41);
    LEARN_EXPECT_EQ(checks, task(), 42);

    std::move_only_function<int(int) const noexcept> doubled = [](int value) noexcept { return value * 2; };
    const auto& const_view = doubled;
    LEARN_EXPECT_EQ(checks, const_view(6), 12);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section05/move_only_function_cpp23",
                                             "std::move_only_function (__cpp_lib_move_only_function >= 202110L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/move_only_function_cpp23", run>;

}  // namespace
