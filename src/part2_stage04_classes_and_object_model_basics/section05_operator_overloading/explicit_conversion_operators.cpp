// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : explicit_conversion_operators
// Topic id : part2/stage04/section05/explicit_conversion_operators
// References: [class.conv.fct]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/explicit_conversion_operators";

class FileDescriptor {
public:
    explicit constexpr FileDescriptor(int value) : value_(value) {}
    explicit constexpr operator bool() const noexcept { return value_ >= 0; }
    explicit constexpr operator int() const noexcept { return value_; }

private:
    int value_;
};

static_assert(!std::is_convertible_v<FileDescriptor, int>);
static_assert(!std::is_convertible_v<FileDescriptor, bool>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr FileDescriptor valid{5};
    constexpr FileDescriptor invalid{-1};

    LEARN_EXPECT(checks, static_cast<bool>(valid));
    LEARN_EXPECT(checks, !static_cast<bool>(invalid));
    LEARN_EXPECT_EQ(checks, static_cast<int>(valid), 5);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section05/explicit_conversion_operators", run>;

}  // namespace
