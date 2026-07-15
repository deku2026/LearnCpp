// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : optional_monadic_operations_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/optional_monadic_operations_cpp23
//
// Covers: optional and_then / transform / or_else C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>
#include <string>
#include <version>

namespace {

std::optional<int> to_int(const std::string& s) {
    if (s == "42") {
        return 42;
    }
    return std::nullopt;
}

void demo_basics() {
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    std::optional<int> o = 2;
    auto t = o.transform([](int x) { return x * 10; });
    assert(t == 20);
#else
    std::optional<int> o = 2;
    assert(*o * 10 == 20);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    std::optional<std::string> s = "42";
    auto n = s.and_then(to_int);
    assert(n == 42);
    std::optional<std::string> bad = "x";
    assert(!bad.and_then(to_int));
#else
    assert(to_int("42") == 42);
    assert(!to_int("x"));
#endif
}

void demo_expert() {
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    auto v = std::optional<int>{}.or_else([] { return std::optional<int>{3}; }).transform([](int x) { return x + 1; });
    assert(v == 4);
    auto empty = std::optional<int>{5}.and_then([](int) -> std::optional<int> { return std::nullopt; });
    assert(!empty);
#else
    std::optional<int> v = 3;
    v = *v + 1;
    assert(v == 4);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/optional_monadic_operations_cpp23", run>;

}  // namespace
