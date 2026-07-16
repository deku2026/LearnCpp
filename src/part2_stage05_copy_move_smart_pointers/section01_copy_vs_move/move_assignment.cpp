// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : move_assignment
// Topic id : part2/stage05/section01/move_assignment
// References: [class.copy.assign]

#include "learn/example_support.hpp"

#include <memory>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/move_assignment";

struct MoveOnlyValue {
    explicit MoveOnlyValue(int value) : data(std::make_unique<int>(value)) {}
    MoveOnlyValue(MoveOnlyValue&&) noexcept = default;
    MoveOnlyValue& operator=(MoveOnlyValue&&) noexcept = default;
    std::unique_ptr<int> data;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    MoveOnlyValue source{12};
    MoveOnlyValue destination{3};
    const int* transferred = source.data.get();
    destination = std::move(source);

    LEARN_EXPECT_EQ(checks, source.data.get(), nullptr);
    LEARN_EXPECT_EQ(checks, destination.data.get(), transferred);
    LEARN_EXPECT_EQ(checks, *destination.data, 12);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/move_assignment", run>;

}  // namespace
