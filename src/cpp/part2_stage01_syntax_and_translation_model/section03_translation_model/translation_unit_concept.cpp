// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : translation_unit_concept
// Topic id : part2/stage01/section03/translation_unit_concept
//
// Covers: one translation unit = one .cpp after includes; internal linkage isolation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

// Included headers become part of THIS translation unit's text stream.
// (topic_registry.hpp was included above — that is the classic "cpp + includes" model.)

namespace {

// Internal linkage: anonymous-namespace entities are unique to this TU.
// If another .cpp also defines `local_counter` in its own anonymous namespace,
// those are different entities — no ODR clash, no shared state.
int local_counter = 0;

void bump_local() {
    ++local_counter;
}

// Simulate two other TUs' internal symbols with nested anonymous namespaces
// (real multi-TU isolation cannot be shown in one file; this models the idea).
namespace simulated_tu_a {
namespace {
int local_counter = 100;
void bump() {
    ++local_counter;
}
int get() {
    return local_counter;
}
}  // namespace
}  // namespace simulated_tu_a

namespace simulated_tu_b {
namespace {
int local_counter = 200;
void bump() {
    ++local_counter;
}
int get() {
    return local_counter;
}
}  // namespace
}  // namespace simulated_tu_b

void demo_basics() {
    // A translation unit is the source file after preprocessing (#includes expanded).
    // This file is one TU; main() lives elsewhere and calls our registered run().
    assert(local_counter == 0);
    bump_local();
    bump_local();
    assert(local_counter == 2);

    // File-scope static (also internal linkage) is another classic TU-private form.
    static int file_static = 0;
    ++file_static;
    assert(file_static == 1);
}

void demo_intermediate() {
    // Same *spelling* of a name in different TUs' internal linkage → independent objects.
    assert(simulated_tu_a::get() == 100);
    assert(simulated_tu_b::get() == 200);

    simulated_tu_a::bump();
    simulated_tu_b::bump();
    simulated_tu_b::bump();

    assert(simulated_tu_a::get() == 101);
    assert(simulated_tu_b::get() == 202);
    // Outer anonymous-namespace counter is still separate.
    assert(local_counter == 2);
}

void demo_expert() {
    // External linkage (not in anonymous ns) would be a single program-wide entity
    // if defined in a header and included from multiple TUs → ODR risk.
    // Safe pattern in headers: inline / static / anonymous ns (C++17 inline vars, etc.).
    const int a = simulated_tu_a::get();
    const int b = simulated_tu_b::get();
    const int c = local_counter;
    assert(a != b);
    assert(c != a);
    assert(c != b);

    // Distinct function entities for each simulated TU's internal helper.
    using Fn = int (*)();
    const Fn fa = &simulated_tu_a::get;
    const Fn fb = &simulated_tu_b::get;
    assert(fa != fb);
    assert(fa() == a);
    assert(fb() == b);

    assert(std::string{"tu"} + std::string{"_model"} == "tu_model");
    (void)std::string_view{"one_cpp_plus_includes"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/translation_unit_concept", run>;

}  // namespace
