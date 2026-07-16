// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : copy_assignment
// Topic id : part2/stage05/section01/copy_assignment
// References: [class.copy.assign]

#include "learn/example_support.hpp"

#include <string>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/copy_assignment";

struct Notebook {
    std::string title;
    std::vector<int> pages;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Notebook source{"source", {1, 2, 3}};
    Notebook destination{"old", {9}};
    destination = source;
    destination.pages[0] = 7;

    LEARN_EXPECT_EQ(checks, destination.title, std::string{"source"});
    LEARN_EXPECT_EQ(checks, destination.pages.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, source.pages[0], 1);  // The vector owns a distinct allocation.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/copy_assignment", run>;

}  // namespace
