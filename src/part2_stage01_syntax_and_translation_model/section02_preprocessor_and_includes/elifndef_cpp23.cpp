// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : elifndef_cpp23
// Topic id : part2/stage01/section02/elifndef_cpp23
// References: N4950 [cpp.cond]; P2334R1.

#include "learn/example_support.hpp"

namespace {

enum class OptionalFacility { unavailable, portable_fallback, accelerator };

// LEARNCPP_STAGE01_OPTIONAL_ACCELERATOR is deliberately absent.
#if defined(LEARNCPP_STAGE01_NEVER_SELECTED)
inline constexpr OptionalFacility selected_by_short_form = OptionalFacility::unavailable;
#elifndef LEARNCPP_STAGE01_OPTIONAL_ACCELERATOR
inline constexpr OptionalFacility selected_by_short_form = OptionalFacility::portable_fallback;
#else
inline constexpr OptionalFacility selected_by_short_form = OptionalFacility::accelerator;
#endif

#if defined(LEARNCPP_STAGE01_NEVER_SELECTED)
inline constexpr OptionalFacility selected_by_long_form = OptionalFacility::unavailable;
#elif !defined(LEARNCPP_STAGE01_OPTIONAL_ACCELERATOR)
inline constexpr OptionalFacility selected_by_long_form = OptionalFacility::portable_fallback;
#else
inline constexpr OptionalFacility selected_by_long_form = OptionalFacility::accelerator;
#endif

static_assert(selected_by_short_form == selected_by_long_form);
static_assert(selected_by_short_form == OptionalFacility::portable_fallback);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section02/elifndef_cpp23"};
    LEARN_EXPECT_EQ(checks, selected_by_short_form, OptionalFacility::portable_fallback);
    LEARN_EXPECT_EQ(checks, selected_by_short_form, selected_by_long_form);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/elifndef_cpp23", run>;

}  // namespace
