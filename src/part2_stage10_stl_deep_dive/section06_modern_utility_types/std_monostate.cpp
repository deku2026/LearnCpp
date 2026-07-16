// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_monostate
// Topic id : part2/stage10/section06/std_monostate

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <variant>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_monostate";

struct Loading {};
struct Ready {
    std::string value;
};
struct Failed {
    int code;
};

using State = std::variant<std::monostate, Loading, Ready, Failed>;

[[nodiscard]] std::string describe(const State& state) {
    return std::visit(
        []<class Alternative>(const Alternative& value) -> std::string {
            using T = std::remove_cvref_t<Alternative>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "not-started";
            } else if constexpr (std::is_same_v<T, Loading>) {
                return "loading";
            } else if constexpr (std::is_same_v<T, Ready>) {
                return "ready:" + value.value;
            } else {
                return "failed:" + std::to_string(value.code);
            }
        },
        state);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    State state;  // monostate makes a naturally empty first alternative possible.
    LEARN_EXPECT(checks, std::holds_alternative<std::monostate>(state));
    LEARN_EXPECT_EQ(checks, state.index(), 0U);
    LEARN_EXPECT_EQ(checks, describe(state), "not-started");

    state.emplace<Loading>();
    LEARN_EXPECT_EQ(checks, describe(state), "loading");
    state.emplace<Ready>("payload");
    LEARN_EXPECT_EQ(checks, describe(state), "ready:payload");
    state.emplace<Failed>(503);
    LEARN_EXPECT_EQ(checks, describe(state), "failed:503");

    static_assert(std::monostate{} == std::monostate{});
    static_assert(std::is_empty_v<std::monostate>);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_monostate", run>;

}  // namespace
