// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : replace_sfinae_with_concepts
// Topic id : part2/stage06/section05/replace_sfinae_with_concepts
//
// Covers: same overload set via enable_if vs concepts

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <string>
#include <type_traits>

namespace {

// Legacy SFINAE style
template <typename T>
std::enable_if_t<std::is_integral_v<T>, int> legacy_kind(T) {
    return 1;
}

template <typename T>
std::enable_if_t<!std::is_integral_v<T>, int> legacy_kind(T) {
    return 2;
}

// Modern concepts style
template <std::integral T>
int modern_kind(T) {
    return 1;
}

template <typename T>
    requires(!std::integral<T>)
int modern_kind(T) {
    return 2;
}

template <typename T>
    requires requires(T t) { t.size(); }
std::size_t modern_size(const T& t) {
    return t.size();
}

void demo_basics() {
    assert(legacy_kind(1) == 1);
    assert(legacy_kind(3.0) == 2);
    assert(modern_kind(1) == 1);
    assert(modern_kind(3.0) == 2);
}

void demo_intermediate() {
    assert(modern_size(std::string{"abc"}) == 3);
    assert(legacy_kind(true) == 1);
    assert(modern_kind(true) == 1);
}

void demo_expert() {
    // Concepts give clearer diagnostics at the call site when constraints fail
    // (compile-time only demonstration via successful constrained calls).
    auto only_signed = [](std::signed_integral auto x) { return x < 0 ? -x : x; };
    assert(only_signed(-5) == 5);
    assert(only_signed(5) == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/replace_sfinae_with_concepts", run>;

}  // namespace
