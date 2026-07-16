// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : unique_ptr_exclusive
// Topic id : part2/stage05/section04/unique_ptr_exclusive
// References: [unique.ptr]

#include "learn/example_support.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/unique_ptr_exclusive";

static_assert(!std::is_copy_constructible_v<std::unique_ptr<int>>);
static_assert(std::is_nothrow_move_constructible_v<std::unique_ptr<int>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto first = std::make_unique<int>(13);
    const int* address = first.get();
    std::unique_ptr<int> second = std::move(first);

    LEARN_EXPECT_EQ(checks, first.get(), nullptr);
    LEARN_EXPECT_EQ(checks, second.get(), address);
    LEARN_EXPECT_EQ(checks, *second, 13);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/unique_ptr_exclusive", run>;

}  // namespace
