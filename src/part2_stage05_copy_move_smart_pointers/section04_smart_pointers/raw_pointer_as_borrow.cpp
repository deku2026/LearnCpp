// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : raw_pointer_as_borrow
// Topic id : part2/stage05/section04/raw_pointer_as_borrow
// References: [basic.compound], C++ Core Guidelines R.3

#include "learn/example_support.hpp"

#include <memory>
#include <span>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/raw_pointer_as_borrow";

void add_one(int* borrowed) {
    if (borrowed != nullptr) {
        ++*borrowed;
    }
}

int sum(std::span<const int> borrowed_range) {
    int result{};
    for (int value : borrowed_range) {
        result += value;
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto owner = std::make_unique<int>(4);
    add_one(owner.get());
    LEARN_EXPECT_EQ(checks, *owner, 5);
    LEARN_EXPECT(checks, owner != nullptr);  // The callee borrowed; it did not delete or store.

    const int values[]{1, 2, 3};
    LEARN_EXPECT_EQ(checks, sum(values), 6);
    add_one(nullptr);  // Optional borrow: null is handled explicitly.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/raw_pointer_as_borrow", run>;

}  // namespace
