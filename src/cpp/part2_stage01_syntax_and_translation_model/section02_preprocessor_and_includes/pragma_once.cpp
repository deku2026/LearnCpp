// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : pragma_once
// Topic id : part2/stage01/section02/pragma_once
//
// Covers: #pragma once vs include guard; single-inclusion semantics (simulated)

#include "learn/topic_registry.hpp"

// Real headers often start with:
//   #pragma once
// which asks the implementation to include this file at most once per TU.
// (Non-standard historically, but universally supported; C++ has no standard
// equivalent to "once" besides include guards.)

// This TU already has #pragma once on topic_registry.hpp; here we *simulate*
// once-vs-guard behaviour with local macros so the lesson is self-contained.

#define LEARNCPP_SIM_ONCE_FILE_ID 0xC0FFEEu

#ifndef LEARNCPP_SIM_ONCE_SEEN
#define LEARNCPP_SIM_ONCE_SEEN
inline constexpr unsigned learncpp_sim_once_payload = LEARNCPP_SIM_ONCE_FILE_ID;
inline constexpr int learncpp_sim_once_hits = 1;
#endif

// Second "include" of the same logical header — skipped when once/guard is set.
#ifndef LEARNCPP_SIM_ONCE_SEEN
#define LEARNCPP_SIM_ONCE_SEEN
inline constexpr unsigned learncpp_sim_once_payload = 0xDEADBEEFu;
inline constexpr int learncpp_sim_once_hits = 2;
#endif

// Belt-and-suspenders pattern many codebases use: both once and a unique guard.
// (In a real .hpp the #pragma once would be at the top of the file.)
#ifndef LEARNCPP_SIM_BELT_AND_SUSPENDERS_HPP
#define LEARNCPP_SIM_BELT_AND_SUSPENDERS_HPP
inline constexpr int learncpp_sim_belt = 1;
#endif

#ifndef LEARNCPP_SIM_BELT_AND_SUSPENDERS_HPP
#define LEARNCPP_SIM_BELT_AND_SUSPENDERS_HPP
inline constexpr int learncpp_sim_belt = 2;
#endif

namespace {

void demo_basics() {
    // After first inclusion, payload is fixed; second paste did not run.
    static_assert(learncpp_sim_once_payload == 0xC0FFEEu);
    LEARN_CHECK(learncpp_sim_once_payload == 0xC0FFEEu);
    LEARN_CHECK(learncpp_sim_once_hits == 1);
}

void demo_intermediate() {
    // Compare approaches (conceptual — both prevent same-TU redefinition):
    // - include guard: portable standard #ifndef/#define/#endif
    // - #pragma once: shorter; compiler tracks file identity (path/inode quirks rare)
    static_assert(learncpp_sim_belt == 1);
    LEARN_CHECK(learncpp_sim_belt == 1);

#if defined(LEARNCPP_SIM_ONCE_SEEN)
    constexpr bool once_model_active = true;
#else
    constexpr bool once_model_active = false;
#endif
    LEARN_CHECK(once_model_active);
}

void demo_expert() {
    // Guards/once only stop re-inclusion inside one TU.
    // Multiple TUs each still paste the header once — need inline/ODR rules for defs.
    constexpr unsigned id = learncpp_sim_once_payload;
    static_assert(id == LEARNCPP_SIM_ONCE_FILE_ID);
    LEARN_CHECK(id == LEARNCPP_SIM_ONCE_FILE_ID);

    // Prefer include guards for maximum portability in public headers;
    // #pragma once is fine for app code when the toolchain is known.
    LEARN_CHECK(learncpp_sim_once_hits == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/pragma_once", run>;

}  // namespace

#undef LEARNCPP_SIM_ONCE_FILE_ID
