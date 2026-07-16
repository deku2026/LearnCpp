// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : include_guard
// Topic id : part2/stage01/section02/include_guard
// References: N4950 [cpp.cond], [cpp.replace].

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

// First encounter with a guarded header body.
#ifndef LEARNCPP_STAGE01_DEMO_HEADER_INCLUDED
#define LEARNCPP_STAGE01_DEMO_HEADER_INCLUDED
namespace guarded_header {
struct Token {
    int value;
};
inline constexpr Token token{17};
}  // namespace guarded_header
inline constexpr int guard_body_entries = 1;
#endif

// A second encounter in the same translation unit skips the body.
#ifndef LEARNCPP_STAGE01_DEMO_HEADER_INCLUDED
inline constexpr bool second_encounter_entered_body = true;
#else
inline constexpr bool second_encounter_entered_body = false;
#endif

#undef LEARNCPP_STAGE01_DEMO_HEADER_INCLUDED

static_assert(guard_body_entries == 1);
static_assert(!second_encounter_entered_body);
static_assert(std::is_same_v<decltype(guarded_header::token), const guarded_header::Token>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section02/include_guard"};
    LEARN_EXPECT_EQ(checks, guard_body_entries, 1);
    LEARN_EXPECT(checks, !second_encounter_entered_body);
    LEARN_EXPECT_EQ(checks, guarded_header::token.value, 17);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/include_guard", run>;

}  // namespace
