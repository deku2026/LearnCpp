// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : copy_construction
// Topic id : part2/stage05/section01/copy_construction
// References: [class.copy.ctor]

#include "learn/example_support.hpp"

#include <memory>
#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/copy_construction";

class DeepValue {
public:
    explicit DeepValue(int value) : value_(std::make_unique<int>(value)) {}
    DeepValue(const DeepValue& other) : value_(std::make_unique<int>(*other.value_)) {}
    DeepValue& operator=(const DeepValue& other) {
        if (this != &other) {
            value_ = std::make_unique<int>(*other.value_);
        }
        return *this;
    }

    [[nodiscard]] int value() const noexcept { return *value_; }
    void set(int value) noexcept { *value_ = value; }
    [[nodiscard]] const int* address() const noexcept { return value_.get(); }

private:
    std::unique_ptr<int> value_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    DeepValue original{5};
    DeepValue copy{original};
    copy.set(8);

    LEARN_EXPECT_EQ(checks, original.value(), 5);
    LEARN_EXPECT_EQ(checks, copy.value(), 8);
    LEARN_EXPECT(checks, original.address() != copy.address());

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/copy_construction", run>;

}  // namespace
