// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : guaranteed_copy_elision_cpp17
// Topic id : part2/stage05/section03/guaranteed_copy_elision_cpp17
// References: [class.copy.elision]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section03/guaranteed_copy_elision_cpp17";

class ImmovableValue {
public:
    explicit constexpr ImmovableValue(int value) : value_(value) {}
    ImmovableValue(const ImmovableValue&) = delete;
    ImmovableValue(ImmovableValue&&) = delete;
    [[nodiscard]] constexpr int value() const noexcept { return value_; }

private:
    int value_;
};

ImmovableValue make_value() {
    return ImmovableValue{17};
}

static_assert(!std::is_copy_constructible_v<ImmovableValue>);
static_assert(!std::is_move_constructible_v<ImmovableValue>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    ImmovableValue value = make_value();
    LEARN_EXPECT_EQ(checks, value.value(), 17);
    // Since C++17 the prvalue initializes `value` directly; no move constructor is required.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section03/guaranteed_copy_elision_cpp17", run>;

}  // namespace
