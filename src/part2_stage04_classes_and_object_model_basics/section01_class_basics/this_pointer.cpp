// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : this_pointer
// Topic id : part2/stage04/section01/this_pointer
// References: [class.this], [class.mfct.non.static]

#include "learn/example_support.hpp"

#include <memory>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section01/this_pointer";

class Counter {
public:
    Counter& add(int delta) noexcept {
        this->value_ += delta;
        return *this;
    }

    [[nodiscard]] const Counter* address() const noexcept { return this; }
    [[nodiscard]] int value() const noexcept { return value_; }

private:
    int value_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Counter counter;
    Counter& chained = counter.add(2).add(3);

    LEARN_EXPECT_EQ(checks, counter.value(), 5);
    LEARN_EXPECT_EQ(checks, std::addressof(chained), std::addressof(counter));
    LEARN_EXPECT_EQ(checks, counter.address(), std::addressof(counter));
    static_assert(std::is_same_v<decltype(counter.add(1)), Counter&>);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section01/this_pointer", run>;

}  // namespace
