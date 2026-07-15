// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : undefined_reference_diagnosis
// Topic id : part2/stage01/section03/undefined_reference_diagnosis
//
// Covers: what causes linker undefined-reference errors (simulated safely in one TU)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

// --- Safe simulation of the "declared but not defined" linker failure ---
//
// BAD (would fail at link if we ODR-used it without a definition):
//   int missing_impl(int x);   // declaration only
//   // no definition anywhere in the program
//   int y = missing_impl(1);   // linker: undefined reference to `missing_impl(int)`
//
// We NEVER leave a used symbol undefined. Instead we:
// 1) show the declaration,
// 2) provide the definition so the build succeeds,
// 3) document diagnosis steps in comments.

int compute_score(int base);  // declaration (as in a header)

// Definition present → no undefined reference at link time.
int compute_score(int base) {
    return base + 10;
}

// Another common cause: forgot to link the .o / library that defines the symbol.
// Modeled as a "provider" module that we deliberately call only after "linking".
namespace provider {
int secret_answer();
}

namespace provider {
int secret_answer() {
    return 42;
}
}  // namespace provider

// Name mangling mismatch (extern "C" vs C++) is another classic undefined-ref cause.
// Document only — both sides use C++ linkage here so we link cleanly.
int mangled_api(int n) {
    return n * 3;
}

struct LinkDiag {
    std::string_view symbol;
    bool definition_found;
};

LinkDiag diagnose(std::string_view symbol, bool has_def) {
    return LinkDiag{symbol, has_def};
}

void demo_basics() {
    // Happy path: declaration + definition → call succeeds.
    assert(compute_score(5) == 15);
    assert(provider::secret_answer() == 42);
}

void demo_intermediate() {
    // Diagnosis workflow (what you do when the linker complains):
    // 1. Read the undefined symbol name (demangle with c++filt / undname).
    // 2. Find who *calls* it (your .o) and who *should define* it (missing .o/lib).
    // 3. Check: wrong signature? forgot .cpp in CMake? not exported from DLL?
    // 4. Fix: add the definition, or link the right library, or match extern "C".

    const LinkDiag ok = diagnose("compute_score(int)", true);
    const LinkDiag bad = diagnose("missing_impl(int)", false);
    assert(ok.definition_found);
    assert(!bad.definition_found);
    assert(ok.symbol.find("compute_score") != std::string_view::npos);

    // Simulated "report" string a developer might search for.
    const std::string linker_msg = "undefined reference to `missing_impl(int)'";
    assert(linker_msg.find("undefined reference") != std::string::npos);
    assert(linker_msg.find("missing_impl") != std::string::npos);
}

void demo_expert() {
    // Checklist of common undefined-reference causes (all avoided here):
    // - Declaration in header, no .cpp definition
    // - Definition in a .cpp not listed in the build
    // - Static library order / --as-needed dropping the provider
    // - C vs C++ linkage mismatch on the same name
    // - Template defined only in .cpp but used from another TU (need header def)
    // - Inline function declared inline but defined only in one TU incorrectly
    //
    // Template case: definition must be visible at instantiation (usually header).
    auto apply = [](auto f, int x) { return f(x); };
    assert(apply(compute_score, 1) == 11);
    assert(apply(mangled_api, 4) == 12);

    // After diagnosis, the fix is always: make exactly one suitable definition
    // reachable to the linker for every ODR-used symbol.
    assert(diagnose("mangled_api(int)", true).definition_found);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/undefined_reference_diagnosis", run>;

}  // namespace
