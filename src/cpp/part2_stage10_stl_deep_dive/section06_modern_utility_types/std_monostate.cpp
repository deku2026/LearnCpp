// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_monostate
// Topic id : part2/stage10/section06_modern_utility_types/std_monostate
//
// Covers: std::monostate empty alternative for variant

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <variant>

namespace {

void demo_basics() {
    std::variant<std::monostate, int, std::string> v;
    LEARN_CHECK(std::holds_alternative<std::monostate>(v));
    LEARN_CHECK(v.index() == 0);
}

void demo_intermediate() {
    std::variant<std::monostate, int> v = 42;
    LEARN_CHECK(std::get<int>(v) == 42);
    v = std::monostate{};
    LEARN_CHECK(std::holds_alternative<std::monostate>(v));
}

void demo_expert() {
    // monostate makes default-constructible variant when no alternative is
    std::variant<std::monostate, std::string> v;
    LEARN_CHECK(!std::holds_alternative<std::string>(v));
    v = "ready";
    LEARN_CHECK(std::get<std::string>(v) == "ready");
    static_assert(std::is_empty_v<std::monostate>);
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_monostate", run>;

}  // namespace
