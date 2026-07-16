// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : move_only_function_as_base_cpp23
// Topic id : part6/e/section06/move_only_function_as_base_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <version>

namespace {

class FallbackMoveOnlyTask {
private:
    struct Interface {
        virtual ~Interface() = default;
        [[nodiscard]] virtual int invoke(int) = 0;
    };

    template <class F>
    struct Model final : Interface {
        explicit Model(F function) : function_(std::move(function)) {}
        [[nodiscard]] int invoke(int value) override { return function_(value); }
        F function_;
    };

public:
    template <class F>
    explicit FallbackMoveOnlyTask(F function) : object_(std::make_unique<Model<F>>(std::move(function))) {}
    FallbackMoveOnlyTask(FallbackMoveOnlyTask&&) noexcept = default;
    FallbackMoveOnlyTask& operator=(FallbackMoveOnlyTask&&) noexcept = default;
    FallbackMoveOnlyTask(const FallbackMoveOnlyTask&) = delete;
    FallbackMoveOnlyTask& operator=(const FallbackMoveOnlyTask&) = delete;
    [[nodiscard]] int operator()(int value) { return object_->invoke(value); }

private:
    std::unique_ptr<Interface> object_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section06/move_only_function_as_base_cpp23"};

    auto state = std::make_unique<int>(2);
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    std::move_only_function<int(int)> task = [owned = std::move(state)](int value) { return value + *owned; };
    static_assert(!std::is_copy_constructible_v<decltype(task)>);
#else
    FallbackMoveOnlyTask task{[owned = std::move(state)](int value) { return value + *owned; }};
    static_assert(!std::is_copy_constructible_v<decltype(task)>);
#endif
    LEARN_EXPECT_EQ(checks, task(40), 42);
    LEARN_EXPECT(checks, state == nullptr);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/move_only_function_as_base_cpp23", run>;

}  // namespace
