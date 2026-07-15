// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : optional_and_expected_versus_magic_value
// Topic id : part2/stage15/section04/optional_and_expected_versus_magic_value
//
// Covers: optional/expected vs magic values

#include "learn/topic_registry.hpp"

#include <optional>
#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<expected>)
#include <expected>
#endif
#endif
namespace {

std::optional<int> parse_positive(int x) {
    if (x <= 0) {
        return std::nullopt;
    }
    return x;
}

void demo_basics() {
    auto a = parse_positive(3);
    LEARN_CHECK(a.has_value() && *a == 3);
    auto b = parse_positive(-1);
    LEARN_CHECK(!b.has_value());
}

void demo_intermediate() {
    // Magic value -1 is ambiguous; optional makes empty explicit.
    int magic = -1;
    LEARN_CHECK(magic == -1);
    std::optional<int> clear;
    LEARN_CHECK(!clear);
}

void demo_expert() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L && __has_include(<expected>)
    std::expected<int, std::string> ok = 42;
    LEARN_CHECK(ok.has_value());
    LEARN_CHECK(*ok == 42);
    std::expected<int, std::string> err = std::unexpected<std::string>{"bad"};
    LEARN_CHECK(!err.has_value());
    LEARN_CHECK(err.error() == "bad");
#else
    std::optional<std::string> e = std::string{"ok"};
    LEARN_CHECK(e.has_value());
    LEARN_CHECK(*e == "ok");
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage15/section04/optional_and_expected_versus_magic_value", run>;

}  // namespace
