// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : rule_of_zero_three_five_idiom
// Topic id : part2/stage15/section01/rule_of_zero_three_five_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section01/rule_of_zero_three_five_idiom";

struct RuleOfZero {
    std::string text;
    std::unique_ptr<int> owner;
};

class RuleOfThree {
public:
    explicit RuleOfThree(int value) : value_(new int(value)) {}
    RuleOfThree(const RuleOfThree& other) : value_(new int(*other.value_)) {}
    RuleOfThree& operator=(const RuleOfThree& other) {
        if (this != &other) *value_ = *other.value_;
        return *this;
    }
    ~RuleOfThree() { delete value_; }
    int value() const { return *value_; }

private:
    int* value_;
};

class RuleOfFive {
public:
    explicit RuleOfFive(int value) : value_(std::make_unique<int>(value)) {}
    RuleOfFive(const RuleOfFive& other) : value_(std::make_unique<int>(*other.value_)) {}
    RuleOfFive(RuleOfFive&&) noexcept = default;
    RuleOfFive& operator=(const RuleOfFive& other) {
        if (this != &other) value_ = std::make_unique<int>(*other.value_);
        return *this;
    }
    RuleOfFive& operator=(RuleOfFive&&) noexcept = default;
    ~RuleOfFive() = default;
    int value() const { return *value_; }

private:
    std::unique_ptr<int> value_;
};

static_assert(!std::copy_constructible<RuleOfZero>);
static_assert(std::copy_constructible<RuleOfThree>);
static_assert(!std::move_constructible<RuleOfThree> || std::copy_constructible<RuleOfThree>);
static_assert(std::move_constructible<RuleOfFive>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    RuleOfThree three{3};
    RuleOfThree three_copy = three;
    RuleOfFive five{5};
    RuleOfFive five_copy = five;
    RuleOfFive five_move = std::move(five_copy);
    LEARN_EXPECT_EQ(checks, three_copy.value(), 3);
    LEARN_EXPECT_EQ(checks, five_move.value(), 5);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section01/rule_of_zero_three_five_idiom", run>;

}  // namespace
