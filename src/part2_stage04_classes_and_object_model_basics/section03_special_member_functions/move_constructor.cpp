// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : move_constructor
// Topic id : part2/stage04/section03/move_constructor
// References: [class.copy.ctor]

#include "learn/example_support.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/move_constructor";

class MessageBuffer {
public:
    explicit MessageBuffer(int value) : value_(std::make_unique<int>(value)) {}
    MessageBuffer(MessageBuffer&&) noexcept = default;
    MessageBuffer& operator=(MessageBuffer&&) noexcept = default;
    MessageBuffer(const MessageBuffer&) = delete;
    MessageBuffer& operator=(const MessageBuffer&) = delete;

    [[nodiscard]] const int* data() const noexcept { return value_.get(); }

private:
    std::unique_ptr<int> value_;
};

static_assert(std::is_nothrow_move_constructible_v<MessageBuffer>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    MessageBuffer source{9};
    const int* original_address = source.data();
    MessageBuffer moved{std::move(source)};

    LEARN_EXPECT_EQ(checks, source.data(), nullptr);
    LEARN_EXPECT_EQ(checks, moved.data(), original_address);
    LEARN_EXPECT_EQ(checks, *moved.data(), 9);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/move_constructor", run>;

}  // namespace
