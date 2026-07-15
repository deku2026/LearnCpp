// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_expected_recap_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/std_expected_recap_cpp23
//
// Covers: std::expected value-or-error recap C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
std::expected<int, std::string> parse(int x) {
    if (x >= 0) {
        return x;
    }
    return std::unexpected("negative");
}
#endif

void demo_basics() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto e = parse(5);
    LEARN_CHECK(e.has_value());
    LEARN_CHECK(*e == 5);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto e = parse(-1);
    LEARN_CHECK(!e);
    LEARN_CHECK(e.error() == "negative");
    LEARN_CHECK(e.value_or(0) == 0);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto e = parse(2).transform([](int x) { return x * 3; });
    LEARN_CHECK(e == 6);
    auto f = parse(-2).transform_error([](const std::string& s) { return s + "!"; });
    LEARN_CHECK(!f && f.error() == "negative!");
#else
    LEARN_CHECK(true);
#endif
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06_modern_utility_types/std_expected_recap_cpp23", run>;

}  // namespace
