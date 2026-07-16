// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : uninitialized_read_deep
// Topic id : part6/g/section03/uninitialized_read_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <optional>
#include <string>

namespace {

struct ParseState {
    std::optional<int> value;
    std::string error;
};

[[nodiscard]] ParseState parse_digit(char character) {
    if (character < '0' || character > '9') {
        return {std::nullopt, "not a digit"};
    }
    return {character - '0', {}};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/uninitialized_read_deep"};

    const std::array<int, 4> value_initialized{};
    LEARN_EXPECT_EQ(checks, value_initialized[0], 0);
    const auto valid = parse_digit('7');
    LEARN_EXPECT_EQ(checks, valid.value, std::optional<int>{7});
    const auto invalid = parse_digit('x');
    LEARN_EXPECT(checks, !invalid.value.has_value());
    LEARN_EXPECT_EQ(checks, invalid.error, std::string{"not a digit"});

#if 0
    int indeterminate;
    return indeterminate;  // Reading an indeterminate int is not a random generator.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/uninitialized_read_deep", run>;

}  // namespace
