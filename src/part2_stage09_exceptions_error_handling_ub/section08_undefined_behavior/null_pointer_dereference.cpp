// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : null_pointer_dereference
// Topic id : part2/stage09/section08/null_pointer_dereference
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <optional>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/null_pointer_dereference";

std::optional<int> read_if_present(const int* pointer) {
    if (pointer == nullptr) {
        return std::nullopt;
    }
    return *pointer;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const int value = 9;
    LEARN_EXPECT_EQ(checks, read_if_present(&value).value(), 9);
    LEARN_EXPECT(checks, !read_if_present(nullptr));
#if 0
    const int undefined = *static_cast<const int*>(nullptr);
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/null_pointer_dereference", run>;

}  // namespace
