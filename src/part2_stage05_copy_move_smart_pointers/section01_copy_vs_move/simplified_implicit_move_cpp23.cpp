// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : simplified_implicit_move_cpp23
// Topic id : part2/stage05/section01/simplified_implicit_move_cpp23
// References: [stmt.return], P2266R3

#include "learn/example_support.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/simplified_implicit_move_cpp23";

struct Token {
    explicit Token(int initial_value) : value(std::make_unique<int>(initial_value)) {}
    Token(Token&&) noexcept = default;
    Token& operator=(Token&&) noexcept = default;
    Token(const Token&) = delete;
    Token& operator=(const Token&) = delete;
    std::unique_ptr<int> value;
};

Token relay(Token parameter) {
    return parameter;  // An eligible automatic object is treated as an xvalue on return.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Token source{31};
    Token result = relay(std::move(source));
    LEARN_EXPECT_EQ(checks, source.value.get(), nullptr);
    LEARN_EXPECT_EQ(checks, *result.value, 31);
    static_assert(!std::is_copy_constructible_v<Token>);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/simplified_implicit_move_cpp23", run>;

}  // namespace
