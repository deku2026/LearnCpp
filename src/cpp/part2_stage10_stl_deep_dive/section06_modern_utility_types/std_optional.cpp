// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_optional
// Topic id : part2/stage10/section06_modern_utility_types/std_optional
//
// Covers: std::optional maybe-value vocabulary type

#include "learn/topic_registry.hpp"

#include <optional>
#include <string>
#include <version>

namespace {

std::optional<int> parse_positive(int x) {
    if (x > 0) {
        return x;
    }
    return std::nullopt;
}

void demo_basics() {
    std::optional<int> o;
    LEARN_CHECK(!o.has_value());
    o = 42;
    LEARN_CHECK(o.has_value());
    LEARN_CHECK(*o == 42);
    LEARN_CHECK(o.value() == 42);
}

void demo_intermediate() {
    auto a = parse_positive(5);
    auto b = parse_positive(-1);
    LEARN_CHECK(a.value_or(0) == 5);
    LEARN_CHECK(b.value_or(0) == 0);
    std::optional<std::string> s = "hi";
    s.reset();
    LEARN_CHECK(!s);
}

void demo_expert() {
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    std::optional<int> o = 10;
    o = o.transform([](int x) { return x + 1; });
    LEARN_CHECK(o == 11);
    auto z = std::optional<int>{}.or_else([] { return std::optional<int>{7}; });
    LEARN_CHECK(z == 7);
#else
    std::optional<int> o = 10;
    o = *o + 1;
    LEARN_CHECK(o == 11);
    std::optional<int> z = 7;
    LEARN_CHECK(z == 7);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_optional", run>;

}  // namespace
