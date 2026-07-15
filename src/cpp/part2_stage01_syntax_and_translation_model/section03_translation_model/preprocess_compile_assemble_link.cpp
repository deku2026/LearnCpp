// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : preprocess_compile_assemble_link
// Topic id : part2/stage01/section03/preprocess_compile_assemble_link
//
// Covers: phases of translation — preprocess, compile, assemble, link — as concepts

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <string_view>

// Preprocess: macros expand before the compiler sees tokens.
#define PHASE_PREPROCESS 1
#define PHASE_COMPILE 2
#define PHASE_ASSEMBLE 3
#define PHASE_LINK 4
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)
#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

namespace {

// Compile: type-checked IR; constexpr / static_assert run here (not at link).
constexpr int pipeline_step_count() {
    return PHASE_PREPROCESS + PHASE_COMPILE + PHASE_ASSEMBLE + PHASE_LINK;
}

static_assert(pipeline_step_count() == 10, "four phases numbered 1..4 sum to 10");
static_assert(PHASE_PREPROCESS < PHASE_COMPILE);
static_assert(PHASE_COMPILE < PHASE_ASSEMBLE);
static_assert(PHASE_ASSEMBLE < PHASE_LINK);

void demo_basics() {
    // Preprocess: STRINGIFY expands args first → "1"; raw # keeps the spelling.
    constexpr const char* expanded = STRINGIFY(PHASE_PREPROCESS);
    assert(std::string_view{expanded} == "1");
#define STRINGIFY_RAW(x) #x
    constexpr const char* raw_name = STRINGIFY_RAW(PHASE_PREPROCESS);
    assert(std::string_view{raw_name} == "PHASE_PREPROCESS");

    constexpr int pasting = CONCAT(12, 34);
    assert(pasting == 1234);

    // Conceptual pipeline order encoded as compile-time constants.
    constexpr int order[] = {PHASE_PREPROCESS, PHASE_COMPILE, PHASE_ASSEMBLE, PHASE_LINK};
    for (int i = 1; i < 4; ++i) {
        assert(order[i] == order[i - 1] + 1);
    }
}

void demo_intermediate() {
    // Compile phase: the compiler sees post-preprocess tokens only.
    // #define values are gone as macros; only their expanded results remain.
    const int a = PHASE_PREPROCESS;
    const int b = PHASE_LINK;
    assert(a == 1);
    assert(b == 4);
    assert(a + b == 5);

    // Assemble phase (concept): each TU becomes an object file with symbols.
    // We model a "symbol table entry" as a string that would appear in an .o.
    const char* defined_symbol = "demo_intermediate";
    assert(std::strlen(defined_symbol) > 0);

    // Link phase (concept): the linker resolves external references across .o files.
    // This single-TU program already has every referenced symbol defined → links cleanly.
    auto add = [](int x, int y) { return x + y; };
    assert(add(PHASE_ASSEMBLE, PHASE_LINK) == 7);
}

void demo_expert() {
    // Map phases to what fails if that stage is broken (teach via comments + asserts).
    // Preprocess fail  → missing include / bad macro → tokens never reach compile.
    // Compile fail     → type error / static_assert → no object file.
    // Assemble fail    → rare (backend); invalid asm / resource.
    // Link fail        → undefined reference / multiple definition across TUs.
    enum class Stage : int { Preprocess = 1, Compile, Assemble, Link };

    auto next = [](Stage s) -> Stage { return static_cast<Stage>(static_cast<int>(s) + 1); };

    assert(static_cast<int>(Stage::Preprocess) == PHASE_PREPROCESS);
    assert(next(Stage::Preprocess) == Stage::Compile);
    assert(next(Stage::Compile) == Stage::Assemble);
    assert(next(Stage::Assemble) == Stage::Link);

    // Feature-test macros are also preprocessor products visible at compile time.
#if defined(__cplusplus)
    static_assert(__cplusplus >= 201103L);
#endif
    assert(pipeline_step_count() == 10);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/preprocess_compile_assemble_link", run>;

}  // namespace
