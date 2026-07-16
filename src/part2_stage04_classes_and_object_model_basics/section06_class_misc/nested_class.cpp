// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : nested_class
// Topic id : part2/stage04/section06/nested_class
// References: [class.nest]

#include "learn/example_support.hpp"

#include <memory>
#include <stdexcept>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section06/nested_class";

class IntStack {
    struct Node {
        int value;
        std::unique_ptr<Node> next;
    };

public:
    void push(int value) { head_ = std::make_unique<Node>(Node{value, std::move(head_)}); }

    [[nodiscard]] int top() const {
        if (!head_) {
            throw std::logic_error{"empty stack"};
        }
        return head_->value;
    }

    [[nodiscard]] bool empty() const noexcept { return head_ == nullptr; }

private:
    std::unique_ptr<Node> head_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    IntStack stack;
    LEARN_EXPECT(checks, stack.empty());
    LEARN_EXPECT_THROWS(checks, std::logic_error, stack.top());
    stack.push(4);
    stack.push(9);
    LEARN_EXPECT_EQ(checks, stack.top(), 9);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section06/nested_class", run>;

}  // namespace
