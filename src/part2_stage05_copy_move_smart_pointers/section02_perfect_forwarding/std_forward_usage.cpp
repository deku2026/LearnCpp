// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : std_forward_usage
// Topic id : part2/stage05/section02/std_forward_usage
// References: [forward]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section02/std_forward_usage";

constexpr std::string_view receive(const std::string&) noexcept {
    return "borrow";
}
constexpr std::string_view receive(std::string&&) noexcept {
    return "consume";
}

template <class T>
constexpr std::string_view relay(T&& value) noexcept {
    return receive(std::forward<T>(value));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::string named = "value";
    LEARN_EXPECT_EQ(checks, relay(named), std::string_view{"borrow"});
    LEARN_EXPECT_EQ(checks, relay(std::string{"temporary"}), std::string_view{"consume"});
    LEARN_EXPECT_EQ(checks, named, std::string{"value"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section02/std_forward_usage", run>;

}  // namespace
