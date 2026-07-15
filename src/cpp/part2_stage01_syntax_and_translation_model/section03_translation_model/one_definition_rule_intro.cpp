// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : one_definition_rule_intro
// Topic id : part2/stage01/section03/one_definition_rule_intro
//
// Covers: ODR intro — one definition for non-inline; inline may appear in multiple TUs

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string_view>

namespace {

// Non-inline function with external linkage: must have exactly one definition
// in the entire program. We define it once in this single TU (safe).
int unique_id() {
    return 1;
}

// Inline function: ODR allows the same definition in multiple TUs if token-identical.
// Headers commonly put small functions as inline so each TU gets a copy; linker merges.
inline int doubled(int x) {
    return x * 2;
}

// Inline variable (C++17): same idea for data.
inline constexpr int kMagic = 0xC0FFEE;

// Simulate "same inline definition appears in two TUs" by calling through two
// identical local wrappers — still one program definition of doubled.
namespace logical_tu_a {
inline int use_doubled(int v) {
    return doubled(v);
}
}  // namespace logical_tu_a

namespace logical_tu_b {
inline int use_doubled(int v) {
    return doubled(v);
}
}  // namespace logical_tu_b

// Template definitions may appear in multiple TUs (implicit inline-ish ODR rules).
template <typename T>
T identity(T v) {
    return v;
}

void demo_basics() {
    assert(unique_id() == 1);
    assert(doubled(21) == 42);
    assert(kMagic == 0xC0FFEE);
}

void demo_intermediate() {
    // Both "TUs" call the same inline function definition — ODR-compliant pattern.
    assert(logical_tu_a::use_doubled(3) == 6);
    assert(logical_tu_b::use_doubled(4) == 8);

    // Function addresses of the same inline may compare equal after linking
    // (implementation may merge); we only assert behavioral identity.
    assert(logical_tu_a::use_doubled(10) == logical_tu_b::use_doubled(10));

    assert(identity(std::string_view{"odr"}) == "odr");
    assert(identity(7) == 7);
}

void demo_expert() {
    // ODR violations (DO NOT do these across real TUs — documented only):
    // 1) Two non-inline definitions of the same function with external linkage.
    // 2) Inline functions with different token sequences in different TUs.
    // 3) Class definitions that disagree on members across TUs.
    //
    // Safe multi-TU patterns:
    // - Put non-inline definitions in exactly one .cpp.
    // - Put small functions as inline in headers (same tokens everywhere).
    // - Prefer anonymous namespace / static for TU-private helpers.

    struct Point {
        int x;
        int y;
        constexpr int sum() const { return x + y; }  // implicit inline in-class
    };

    constexpr Point p{3, 4};
    static_assert(p.sum() == 7);
    assert(p.sum() == 7);

    // One definition of unique_id in this program — calling it repeatedly is fine.
    assert(unique_id() + unique_id() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/one_definition_rule_intro", run>;

}  // namespace
