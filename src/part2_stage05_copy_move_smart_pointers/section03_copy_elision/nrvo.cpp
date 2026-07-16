// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : nrvo
// Topic id : part2/stage05/section03/nrvo
// References: [class.copy.elision]

#include "learn/example_support.hpp"

#include <string>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section03/nrvo";

struct NamedResult {
    explicit NamedResult(std::string input) : value(std::move(input)) {}
    NamedResult(const NamedResult& other) : value(other.value) { ++copies; }
    NamedResult(NamedResult&& other) noexcept : value(std::move(other.value)) { ++moves; }

    std::string value;
    inline static int copies{};
    inline static int moves{};
};

NamedResult make_named() {
    NamedResult result{"named local"};
    return result;  // NRVO may construct directly in the caller.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    NamedResult::copies = 0;
    NamedResult::moves = 0;
    NamedResult result = make_named();

    LEARN_EXPECT_EQ(checks, result.value, std::string{"named local"});
    LEARN_EXPECT_EQ(checks, NamedResult::copies, 0);
    LEARN_EXPECT(checks, NamedResult::moves <= 1);  // Zero with NRVO, one with implicit move.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section03/nrvo", run>;

}  // namespace
