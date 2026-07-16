// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : explicit_constructor
// Topic id : part2/stage04/section01/explicit_constructor
// References: [dcl.fct.spec], [over.match.copy]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section01/explicit_constructor";

class Seconds {
public:
    explicit constexpr Seconds(int value) : value_(value) {}
    [[nodiscard]] constexpr int count() const noexcept { return value_; }

private:
    int value_;
};

constexpr int twice(Seconds value) {
    return value.count() * 2;
}

static_assert(std::is_constructible_v<Seconds, int>);
static_assert(!std::is_convertible_v<int, Seconds>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr Seconds timeout{5};
    static_assert(twice(timeout) == 10);
    LEARN_EXPECT_EQ(checks, twice(Seconds{7}), 14);

    // twice(7) would be ill-formed: explicit blocks the unintended implicit conversion.
    LEARN_EXPECT(checks, (!std::is_convertible_v<int, Seconds>));

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section01/explicit_constructor", run>;

}  // namespace
