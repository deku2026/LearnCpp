// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_control_block
// Topic id : part2/stage05/section04/shared_ptr_control_block
// References: [util.smartptr.shared.const]

#include "learn/example_support.hpp"

#include <memory>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/shared_ptr_control_block";

struct Pair {
    int first;
    int second;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto owner = std::make_shared<Pair>(Pair{3, 8});
    std::shared_ptr<int> alias{owner, &owner->second};

    LEARN_EXPECT_EQ(checks, alias.get(), &owner->second);
    LEARN_EXPECT_EQ(checks, owner.use_count(), 2L);
    owner.reset();
    LEARN_EXPECT_EQ(checks, *alias, 8);  // Aliasing pointer shares the same control block.
    LEARN_EXPECT_EQ(checks, alias.use_count(), 1L);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/shared_ptr_control_block", run>;

}  // namespace
