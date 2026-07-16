// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_shared
// Topic id : part2/stage05/section04/shared_ptr_shared
// References: [util.smartptr.shared]

#include "learn/example_support.hpp"

#include <memory>
#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/shared_ptr_shared";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto first = std::make_shared<std::string>("shared state");
    LEARN_EXPECT_EQ(checks, first.use_count(), 1L);
    {
        std::shared_ptr<std::string> second = first;
        LEARN_EXPECT_EQ(checks, first.get(), second.get());
        LEARN_EXPECT_EQ(checks, first.use_count(), 2L);
        second->append(" updated");
    }
    LEARN_EXPECT_EQ(checks, first.use_count(), 1L);
    LEARN_EXPECT_EQ(checks, *first, std::string{"shared state updated"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/shared_ptr_shared", run>;

}  // namespace
