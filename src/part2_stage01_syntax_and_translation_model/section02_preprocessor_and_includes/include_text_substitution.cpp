// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : include_text_substitution
// Topic id : part2/stage01/section02/include_text_substitution
// References: N4950 [cpp.include], [lex.phases].

#include "learn/example_support.hpp"

#include <array>

namespace {

#if __has_include("learn/example_support.hpp")
inline constexpr bool quoted_project_header_is_reachable = true;
#else
inline constexpr bool quoted_project_header_is_reachable = false;
#endif

#if __has_include(<array>)
inline constexpr bool angled_library_header_is_reachable = true;
#else
inline constexpr bool angled_library_header_is_reachable = false;
#endif

// After preprocessing, this name is replaced by the token 6 before parsing.
#define LEARNCPP_STAGE01_INCLUDED_FACTOR 6
inline constexpr int value_after_macro_replacement = LEARNCPP_STAGE01_INCLUDED_FACTOR * 7;
#undef LEARNCPP_STAGE01_INCLUDED_FACTOR

enum class IncludeSpelling { quoted, angled };

struct IncludeSearchModel {
    IncludeSpelling spelling;
    bool implementation_defined_search;
    bool commonly_checks_near_including_file_first;
};

inline constexpr std::array include_forms{
    IncludeSearchModel{IncludeSpelling::quoted, true, true},
    IncludeSearchModel{IncludeSpelling::angled, true, false},
};

static_assert(quoted_project_header_is_reachable);
static_assert(angled_library_header_is_reachable);
static_assert(value_after_macro_replacement == 42);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section02/include_text_substitution"};
    LEARN_EXPECT(checks, quoted_project_header_is_reachable);
    LEARN_EXPECT(checks, angled_library_header_is_reachable);
    LEARN_EXPECT_EQ(checks, value_after_macro_replacement, 42);
    LEARN_EXPECT_EQ(checks, include_forms.size(), 2uz);
    LEARN_EXPECT_EQ(checks, include_forms[0].spelling, IncludeSpelling::quoted);
    LEARN_EXPECT(checks, include_forms[0].commonly_checks_near_including_file_first);
    LEARN_EXPECT(checks, include_forms[1].implementation_defined_search);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/include_text_substitution", run>;

}  // namespace
