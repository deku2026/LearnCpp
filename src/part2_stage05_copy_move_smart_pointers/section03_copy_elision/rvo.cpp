// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : rvo
// Topic id : part2/stage05/section03/rvo
// References: [class.copy.elision]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section03/rvo";

struct DirectResult {
    explicit DirectResult(int input) : value(input) {}
    DirectResult(const DirectResult&) = delete;
    DirectResult(DirectResult&&) = delete;
    int value;
};

DirectResult make_direct() {
    return DirectResult{23};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    DirectResult result = make_direct();
    LEARN_EXPECT_EQ(checks, result.value, 23);
    static_assert(!std::is_move_constructible_v<DirectResult>);
    // This traditional "RVO" case is mandatory prvalue materialization since C++17.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section03/rvo", run>;

}  // namespace
