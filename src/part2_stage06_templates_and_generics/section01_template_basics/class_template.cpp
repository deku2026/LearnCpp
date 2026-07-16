// A class template generates an independent class for every argument list.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : class_template
// Topic id : part2/stage06/section01/class_template
//
// This fixed-capacity stack keeps its bounds explicit and reports empty/full
// states without invoking undefined behavior.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>

namespace {

template <class T, std::size_t Capacity>
class FixedStack {
public:
    bool push(T value) {
        if (size_ == Capacity) {
            return false;
        }
        values_[size_++] = std::move(value);
        return true;
    }

    std::optional<T> pop() {
        if (size_ == 0) {
            return std::nullopt;
        }
        return std::move(values_[--size_]);
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }

private:
    std::array<T, Capacity> values_{};
    std::size_t size_{};
};

static_assert(!std::is_same_v<FixedStack<int, 2>, FixedStack<double, 2>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/class_template"};
    FixedStack<int, 2> stack;
    LEARN_EXPECT(checks, stack.push(10));
    LEARN_EXPECT(checks, stack.push(20));
    LEARN_EXPECT(checks, !stack.push(30));
    LEARN_EXPECT_EQ(checks, stack.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, stack.pop().value(), 20);
    LEARN_EXPECT_EQ(checks, stack.pop().value(), 10);
    LEARN_EXPECT(checks, !stack.pop().has_value());
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/class_template", run>;

}  // namespace
