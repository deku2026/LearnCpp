// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : never_move_return_of_local
// Topic id : part2/stage05/section03/never_move_return_of_local
// References: [class.copy.elision], [stmt.return]

#include "learn/example_support.hpp"

#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section03/never_move_return_of_local";

struct TracedResult {
    explicit TracedResult(int input) : value(input) {}
    TracedResult(const TracedResult& other) : value(other.value) { ++copies; }
    TracedResult(TracedResult&& other) noexcept : value(std::exchange(other.value, 0)) { ++moves; }

    int value;
    inline static int copies{};
    inline static int moves{};
};

TracedResult allow_elision() {
    TracedResult local{5};
    return local;
}

#if 0
TracedResult pessimizing_return() {
    TracedResult local{7};
    return std::move(local);  // Bad: blocks NRVO and triggers -Wpessimizing-move.
}
#endif

TracedResult explicit_move_for_comparison() {
    TracedResult local{7};
    TracedResult moved{std::move(local)};  // An explicit move outside a return statement.
    return moved;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    TracedResult::copies = 0;
    TracedResult::moves = 0;
    TracedResult preferred = allow_elision();
    const int preferred_moves = TracedResult::moves;
    LEARN_EXPECT_EQ(checks, preferred.value, 5);
    LEARN_EXPECT_EQ(checks, TracedResult::copies, 0);
    LEARN_EXPECT(checks, preferred_moves <= 1);  // NRVO is optional; implicit move is the fallback.

    TracedResult::moves = 0;
    TracedResult compared = explicit_move_for_comparison();
    LEARN_EXPECT_EQ(checks, compared.value, 7);
    LEARN_EXPECT(checks, TracedResult::moves >= 1);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section03/never_move_return_of_local", run>;

}  // namespace
