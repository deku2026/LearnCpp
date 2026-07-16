// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : valid_but_unspecified_state
// Topic id : part2/stage05/section01/valid_but_unspecified_state
// References: [lib.types.movedfrom]

#include "learn/example_support.hpp"

#include <string>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/valid_but_unspecified_state";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::vector<int> source{1, 2, 3};
    std::vector<int> destination = std::move(source);
    LEARN_EXPECT_EQ(checks, destination.size(), std::size_t{3});

    // Do not assert that source is empty: its exact value is generally unspecified.
    source.clear();       // Operations without preconditions remain valid.
    source.push_back(9);  // It can be assigned to or reused.
    LEARN_EXPECT_EQ(checks, source.front(), 9);
    LEARN_EXPECT_EQ(checks, source.size(), std::size_t{1});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/valid_but_unspecified_state", run>;

}  // namespace
