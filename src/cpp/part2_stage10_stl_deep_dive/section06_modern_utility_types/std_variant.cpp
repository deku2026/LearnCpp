// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_variant
// Topic id : part2/stage10/section06_modern_utility_types/std_variant
//
// Covers: std::variant type-safe union

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <variant>

namespace {

void demo_basics() {
    std::variant<int, std::string> v = 42;
    LEARN_CHECK(std::holds_alternative<int>(v));
    LEARN_CHECK(std::get<int>(v) == 42);
    v = "hi";
    LEARN_CHECK(std::get<std::string>(v) == "hi");
}

void demo_intermediate() {
    std::variant<int, double, std::string> v = 3.14;
    LEARN_CHECK(v.index() == 1);
    LEARN_CHECK(std::get_if<int>(&v) == nullptr);
    LEARN_CHECK(std::get_if<double>(&v) != nullptr);
}

void demo_expert() {
    std::variant<int, std::string> v = 7;
    auto s = std::visit(
        [](auto&& x) -> std::string {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, int>) {
                return std::to_string(x);
            } else {
                return x;
            }
        },
        v);
    LEARN_CHECK(s == "7");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_variant", run>;

}  // namespace
