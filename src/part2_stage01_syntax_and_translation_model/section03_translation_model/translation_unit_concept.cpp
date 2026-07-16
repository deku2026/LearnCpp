// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : translation_unit_concept
// Topic id : part2/stage01/section03/translation_unit_concept
// References: N4950 [lex.separate], [lex.phases].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

enum class SourceRole { primary_source, included_header, discarded_conditional_group };

struct SourceContribution {
    SourceRole role;
    bool retained_after_preprocessing;
    std::size_t declaration_count;
};

inline constexpr std::array contributions{
    SourceContribution{SourceRole::primary_source, true, 2},
    SourceContribution{SourceRole::included_header, true, 3},
    SourceContribution{SourceRole::discarded_conditional_group, false, 7},
};

constexpr std::size_t retained_declaration_count() noexcept {
    std::size_t result = 0;
    for (const auto& contribution : contributions) {
        if (contribution.retained_after_preprocessing) {
            result += contribution.declaration_count;
        }
    }
    return result;
}

constexpr bool independently_starts_this_translation_unit(SourceRole role) noexcept {
    return role == SourceRole::primary_source;
}

static_assert(retained_declaration_count() == 5);
static_assert(independently_starts_this_translation_unit(SourceRole::primary_source));
static_assert(!independently_starts_this_translation_unit(SourceRole::included_header));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/translation_unit_concept"};

    LEARN_EXPECT_EQ(checks, retained_declaration_count(), 5uz);
    LEARN_EXPECT(checks, independently_starts_this_translation_unit(SourceRole::primary_source));
    LEARN_EXPECT(checks, !independently_starts_this_translation_unit(SourceRole::included_header));
    LEARN_EXPECT(checks, !contributions[2].retained_after_preprocessing);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/translation_unit_concept", run>;

}  // namespace
