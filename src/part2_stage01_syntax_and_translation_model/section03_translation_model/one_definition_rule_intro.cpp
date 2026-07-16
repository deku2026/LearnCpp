// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : one_definition_rule_intro
// Topic id : part2/stage01/section03/one_definition_rule_intro
// References: N4950 [basic.def.odr], [dcl.inline].

#include "learn/example_support.hpp"

#include <array>

namespace {

enum class EntityKind { non_inline_function, inline_function, class_type, templated_entity };
enum class ProgramWideMultiplicity { one_definition, corresponding_definitions_permitted };

struct OdrCategory {
    EntityKind entity;
    ProgramWideMultiplicity multiplicity;
    bool at_most_one_definition_per_translation_unit;
    bool repeated_definitions_have_strict_correspondence_requirements;
};

inline constexpr std::array categories{
    OdrCategory{EntityKind::non_inline_function, ProgramWideMultiplicity::one_definition, true, false},
    OdrCategory{EntityKind::inline_function, ProgramWideMultiplicity::corresponding_definitions_permitted, true, true},
    OdrCategory{EntityKind::class_type, ProgramWideMultiplicity::corresponding_definitions_permitted, true, true},
    OdrCategory{EntityKind::templated_entity, ProgramWideMultiplicity::corresponding_definitions_permitted, true, true},
};

struct HeaderRecord {
    int value;
};

inline constexpr int header_square(int value) noexcept {
    return value * value;
}

template <class Value>
constexpr Value twice(Value value) {
    return value + value;
}

// If this ordinary external definition were placed in a header and included
// by two translation units, the resulting program would violate the ODR.
#if 0
int ordinary_header_definition() {
    return 42;
}
#endif

static_assert(categories[0].multiplicity == ProgramWideMultiplicity::one_definition);
static_assert(categories[0].at_most_one_definition_per_translation_unit);
static_assert(categories[1].repeated_definitions_have_strict_correspondence_requirements);
static_assert(header_square(6) == 36);
static_assert(twice(HeaderRecord{21}.value) == 42);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/one_definition_rule_intro"};

    LEARN_EXPECT_EQ(checks, categories.size(), 4uz);
    LEARN_EXPECT_EQ(checks, categories[0].multiplicity, ProgramWideMultiplicity::one_definition);
    LEARN_EXPECT_EQ(checks, categories[1].multiplicity, ProgramWideMultiplicity::corresponding_definitions_permitted);
    LEARN_EXPECT(checks, categories[2].at_most_one_definition_per_translation_unit);
    LEARN_EXPECT_EQ(checks, header_square(7), 49);
    LEARN_EXPECT_EQ(checks, twice(21), 42);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/one_definition_rule_intro", run>;

}  // namespace
