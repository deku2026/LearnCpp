// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : noexcept_move_and_vector_growth
// Topic id : part2/stage05/section01/noexcept_move_and_vector_growth
// References: [vector.modifiers], [class.copy.ctor]

#include "learn/example_support.hpp"

#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/noexcept_move_and_vector_growth";

class Relocatable {
public:
    explicit Relocatable(int value) : value_(value) {}
    Relocatable(const Relocatable& other) : value_(other.value_) { ++copies; }
    Relocatable(Relocatable&& other) noexcept : value_(std::exchange(other.value_, 0)) { ++moves; }
    Relocatable& operator=(const Relocatable&) = default;
    Relocatable& operator=(Relocatable&&) noexcept = default;

    [[nodiscard]] int value() const noexcept { return value_; }

    inline static int copies{};
    inline static int moves{};

private:
    int value_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Relocatable::copies = 0;
    Relocatable::moves = 0;
    std::vector<Relocatable> values;
    values.reserve(1);
    values.emplace_back(7);
    values.emplace_back(9);  // Reallocation prefers the noexcept move constructor.

    LEARN_EXPECT_EQ(checks, Relocatable::copies, 0);
    LEARN_EXPECT(checks, Relocatable::moves >= 1);
    LEARN_EXPECT_EQ(checks, values.front().value(), 7);

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage05/section01/noexcept_move_and_vector_growth", run>;

}  // namespace
