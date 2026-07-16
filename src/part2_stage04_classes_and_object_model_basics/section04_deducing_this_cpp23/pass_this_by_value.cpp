// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : pass_this_by_value
// Topic id : part2/stage04/section04/pass_this_by_value
// References: [dcl.fct], P0847R7

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section04/pass_this_by_value";

struct SmallValue {
    int value{};

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    [[nodiscard]] int plus(this SmallValue self, int delta) noexcept {
        self.value += delta;
        return self.value;
    }
#else
    [[nodiscard]] int plus(int delta) const noexcept {
        SmallValue copy = *this;
        copy.value += delta;
        return copy.value;
    }
#endif
};

static_assert(std::is_trivially_copyable_v<SmallValue>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    SmallValue original{10};
    LEARN_EXPECT_EQ(checks, original.plus(5), 15);
    LEARN_EXPECT_EQ(checks, original.value, 10);  // The explicit object parameter was a copy.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section04/pass_this_by_value", run>;

}  // namespace
