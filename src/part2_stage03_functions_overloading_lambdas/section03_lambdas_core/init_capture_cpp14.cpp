// C++14 init-capture creates and initializes a new closure data member.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : init_capture_cpp14
// Topic id : part2/stage03/section03/init_capture_cpp14
//
// It supports renaming, computed values, and moving an otherwise non-copyable
// resource into the closure.

#include "learn/example_support.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/init_capture_cpp14"};

    auto resource = std::make_unique<int>(21);
    auto task = [owned = std::move(resource), multiplier = 2] { return *owned * multiplier; };
    static_assert(std::is_move_constructible_v<decltype(task)>);
    static_assert(!std::is_copy_constructible_v<decltype(task)>);
    LEARN_EXPECT(checks, resource == nullptr);

    auto moved_task = std::move(task);
    LEARN_EXPECT_EQ(checks, moved_task(), 42);

    int source = 5;
    const auto computed = [twice = source * 2] { return twice; };
    source = 9;
    LEARN_EXPECT_EQ(checks, computed(), 10);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/init_capture_cpp14", run>;

}  // namespace
