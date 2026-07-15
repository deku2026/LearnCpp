// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : optional_and_expected_versus_magic_value
// Topic id : part2/stage15/section04/optional_and_expected_versus_magic_value
//
// Covers: optional/expected vs magic values

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>
#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
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
    assert(a.has_value() && *a == 3);
    auto b = parse_positive(-1);
    assert(!b.has_value());
}

void demo_intermediate() {
    // Magic value -1 is ambiguous; optional makes empty explicit.
    int magic = -1;
    assert(magic == -1);
    std::optional<int> clear;
    assert(!clear);
}

void demo_expert() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    std::expected<int, std::string> ok = 42;
    assert(ok.has_value());
    assert(*ok == 42);
    std::expected<int, std::string> err = std::unexpected<std::string>{"bad"};
    assert(!err.has_value());
    assert(err.error() == "bad");
#else
    std::optional<std::string> e = std::string{"ok"};
    assert(e.has_value());
    assert(*e == "ok");
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
