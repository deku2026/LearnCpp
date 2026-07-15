// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §4.5
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : optional_and_expected_versus_magic_value
// Topic id : part2/stage15/section04/optional_and_expected_versus_magic_value
//
// optional / expected vs magic return values / output params.
// Motive : encode "maybe none" / "maybe error" in the type system.
// Modern : C++23 expected monadic and_then / transform railway-oriented flow.
// Pitfall: optional loses error cause; expected is not a substitute for true exceptions.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <version>

#if !defined(__cpp_lib_expected) || !(__cpp_lib_expected) || !__has_include(<expected>)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::expected not available (__cpp_lib_expected)\n";
    return 0;
}
[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage15/section04/optional_and_expected_versus_magic_value", run>;
}  // namespace
#else
#include <expected>
namespace {

// ---------- bad: magic values ----------
int parse_magic(std::string_view s) {
    if (s == "ok") {
        return 42;
    }
    return -1;  // easy to ignore; -1 might be a valid domain value elsewhere
}

// ---------- optional: presence only ----------
std::optional<int> find_id(bool present) {
    if (present) {
        return 7;
    }
    return std::nullopt;
}

// ---------- expected: value or error ----------
std::expected<int, std::string> parse(std::string_view s) {
    if (s == "ok") {
        return 42;
    }
    return std::unexpected(std::string{"parse failed: "} + std::string{s});
}

std::expected<int, std::string> ensure_large(int v) {
    if (v > 50) {
        return v;
    }
    return std::unexpected(std::string{"too small"});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== magic value (legacy) ==\n";
    int m = parse_magic("x");
    if (m < 0) {
        std::cout << "  failed (but caller must remember the convention)\n";
    }

    std::cout << "== optional ==\n";
    if (auto id = find_id(true)) {
        std::cout << "  found " << *id << '\n';
        assert(*id == 7);
    }
    assert(!find_id(false).has_value());

    std::cout << "== expected + railway (C++23) ==\n";
    auto ok = parse("ok")
                  .transform([](int v) { return v * 2; })   // 84
                  .and_then(ensure_large)                   // still 84
                  .transform([](int v) { return v + 1; });  // 85
    assert(ok.has_value());
    assert(*ok == 85);
    std::cout << "  success: " << *ok << '\n';

    auto bad = parse("nope").transform([](int v) { return v * 2; });
    assert(!bad.has_value());
    std::cout << "  failure short-circuits: " << bad.error() << '\n';

    auto small = parse("ok").transform([](int v) { return v; }).and_then(ensure_large);
    assert(!small.has_value());
    std::cout << "  pipeline error: " << small.error() << '\n';

    std::cout << "optional = maybe value; expected = value or reason; exceptions = rare failures\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage15/section04/optional_and_expected_versus_magic_value", run>;

}  // namespace
#endif  // __cpp_lib_expected
