// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : elifdef_cpp23
// Topic id : part2/stage01/section02/elifdef_cpp23
// References: N4950 [cpp.cond]; P2334R1.

#include "learn/example_support.hpp"

namespace {

enum class SelectedBackend { unavailable, cpu, gpu };

#define LEARNCPP_STAGE01_CPU_BACKEND

// C++23 #elifdef X is exactly the short form of #elif defined(X).
#if defined(LEARNCPP_STAGE01_NEVER_SELECTED)
inline constexpr SelectedBackend selected_by_short_form = SelectedBackend::unavailable;
#elifdef LEARNCPP_STAGE01_CPU_BACKEND
inline constexpr SelectedBackend selected_by_short_form = SelectedBackend::cpu;
#else
inline constexpr SelectedBackend selected_by_short_form = SelectedBackend::gpu;
#endif

#if defined(LEARNCPP_STAGE01_NEVER_SELECTED)
inline constexpr SelectedBackend selected_by_long_form = SelectedBackend::unavailable;
#elif defined(LEARNCPP_STAGE01_CPU_BACKEND)
inline constexpr SelectedBackend selected_by_long_form = SelectedBackend::cpu;
#else
inline constexpr SelectedBackend selected_by_long_form = SelectedBackend::gpu;
#endif

#undef LEARNCPP_STAGE01_CPU_BACKEND

static_assert(selected_by_short_form == selected_by_long_form);
static_assert(selected_by_short_form == SelectedBackend::cpu);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section02/elifdef_cpp23"};
    LEARN_EXPECT_EQ(checks, selected_by_short_form, SelectedBackend::cpu);
    LEARN_EXPECT_EQ(checks, selected_by_short_form, selected_by_long_form);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/elifdef_cpp23", run>;

}  // namespace
