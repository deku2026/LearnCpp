// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : named_universal_character_escape
// Topic id : part2/stage02/section10/named_universal_character_escape
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_named_character_escapes) && __cpp_named_character_escapes >= 202207L
    constexpr char32_t named = U'\N{LATIN CAPITAL LETTER A}';
    constexpr char32_t numeric = U'\u0041';
    static_assert(std::is_same_v<decltype(named), const char32_t>);

    learn::ExampleChecks checks{"part2/stage02/section10/named_universal_character_escape"};
    LEARN_EXPECT_EQ(checks, named, numeric);
    LEARN_EXPECT_EQ(checks, named, U'A');
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section10/named_universal_character_escape",
                                             "__cpp_named_character_escapes");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/named_universal_character_escape", run>;

}  // namespace
