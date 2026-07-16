// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section07_keyword_table
// Item     : keyword_taxonomy_a_to_p
// Topic id : part2/stage14/section07/keyword_taxonomy_a_to_p
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section07/keyword_taxonomy_a_to_p";

enum class Category { declaration, type, control, expression, coroutine, alternative_token };

struct KeywordGroup {
    std::string_view words;
    Category category;
};

constexpr std::array groups{
    KeywordGroup{"alignas alignof asm auto", Category::declaration},
    KeywordGroup{"bitand bitor compl not not_eq or or_eq xor xor_eq", Category::alternative_token},
    KeywordGroup{"bool char char8_t char16_t char32_t double float int long short signed unsigned void wchar_t",
                 Category::type},
    KeywordGroup{"break case catch continue default do else for goto if return switch throw try while",
                 Category::control},
    KeywordGroup{"class concept const consteval constexpr constinit enum explicit export extern friend inline mutable "
                 "namespace private protected public",
                 Category::declaration},
    KeywordGroup{"co_await co_return co_yield", Category::coroutine},
    KeywordGroup{"decltype delete dynamic_cast false new noexcept nullptr operator reinterpret_cast sizeof static_cast "
                 "this true typeid",
                 Category::expression},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, groups.size(), 7U);
    LEARN_EXPECT(checks, std::ranges::any_of(groups, [](const KeywordGroup& group) {
                     return group.category == Category::coroutine && group.words.contains("co_yield");
                 }));
    LEARN_EXPECT(checks, std::ranges::any_of(groups, [](const KeywordGroup& group) {
                     return group.category == Category::alternative_token && group.words.contains("bitand");
                 }));
    // Context-sensitive identifiers such as final, override and import are intentionally not classified as keywords
    // here.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage14/section07/keyword_taxonomy_a_to_p", run>;

}  // namespace
