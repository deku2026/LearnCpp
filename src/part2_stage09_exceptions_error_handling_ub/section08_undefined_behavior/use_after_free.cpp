// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : use_after_free
// Topic id : part2/stage09/section08/use_after_free
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/use_after_free";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto owner = std::make_shared<int>(42);
    std::weak_ptr<int> observer = owner;
    LEARN_EXPECT_EQ(checks, *observer.lock(), 42);
    owner.reset();
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT(checks, observer.lock() == nullptr);
#if 0
    int* raw = new int{42};
    delete raw;
    const int undefined = *raw;
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/use_after_free", run>;

}  // namespace
