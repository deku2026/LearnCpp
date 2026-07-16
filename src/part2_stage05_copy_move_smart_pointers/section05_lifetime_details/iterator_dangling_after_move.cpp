// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : iterator_dangling_after_move
// Topic id : part2/stage05/section05/iterator_dangling_after_move
// References: [container.requirements.general], [vector.cons]

#include "learn/example_support.hpp"

#include <string>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section05/iterator_dangling_after_move";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::vector<std::string> source{"alpha", "beta"};
    std::vector<std::string> destination = std::move(source);

    // Reacquire iterators from the object that now owns the elements.  Exact moved-from
    // state is unspecified, and move-assignment invalidation can depend on allocators.
    const auto current = destination.begin();
    LEARN_EXPECT_EQ(checks, *current, std::string{"alpha"});
    LEARN_EXPECT_EQ(checks, destination.size(), std::size_t{2});

    source.assign({"reused"});
    LEARN_EXPECT_EQ(checks, source.front(), std::string{"reused"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section05/iterator_dangling_after_move", run>;

}  // namespace
