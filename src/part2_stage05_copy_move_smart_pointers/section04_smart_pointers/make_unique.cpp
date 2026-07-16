// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_unique
// Topic id : part2/stage05/section04/make_unique
// References: [unique.ptr.create]

#include "learn/example_support.hpp"

#include <memory>
#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/make_unique";

struct User {
    std::string name;
    int score{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto user = std::make_unique<User>(User{"Ada", 99});
    auto values = std::make_unique<int[]>(3);
    values[0] = 2;
    values[1] = 3;
    values[2] = 5;

    LEARN_EXPECT_EQ(checks, user->name, std::string{"Ada"});
    LEARN_EXPECT_EQ(checks, user->score, 99);
    LEARN_EXPECT_EQ(checks, values[0] + values[1] + values[2], 10);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/make_unique", run>;

}  // namespace
