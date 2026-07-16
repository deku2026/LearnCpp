// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : move_assignment
// Topic id : part2/stage04/section03/move_assignment
// References: [class.copy.assign]

#include "learn/example_support.hpp"

#include <memory>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/move_assignment";

class Payload {
public:
    explicit Payload(int value) : value_(std::make_unique<int>(value)) {}
    Payload(Payload&&) noexcept = default;
    Payload& operator=(Payload&&) noexcept = default;
    Payload(const Payload&) = delete;
    Payload& operator=(const Payload&) = delete;

    [[nodiscard]] bool has_value() const noexcept { return value_ != nullptr; }
    [[nodiscard]] int value() const noexcept { return *value_; }

private:
    std::unique_ptr<int> value_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Payload source{42};
    Payload destination{7};
    destination = std::move(source);

    LEARN_EXPECT(checks, !source.has_value());
    LEARN_EXPECT(checks, destination.has_value());
    LEARN_EXPECT_EQ(checks, destination.value(), 42);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/move_assignment", run>;

}  // namespace
