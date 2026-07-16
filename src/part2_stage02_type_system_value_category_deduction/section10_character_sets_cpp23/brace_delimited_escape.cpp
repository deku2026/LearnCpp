// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : brace_delimited_escape
// Topic id : part2/stage02/section10/brace_delimited_escape
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_named_character_escapes) && __cpp_named_character_escapes >= 202207L
    constexpr char hexadecimal = '\x{41}';
    constexpr char octal = '\o{101}';
    constexpr char32_t universal = U'\u{41}';
    static_assert(std::is_same_v<decltype(universal), const char32_t>);

    learn::ExampleChecks checks{"part2/stage02/section10/brace_delimited_escape"};
    LEARN_EXPECT_EQ(checks, hexadecimal, 'A');
    LEARN_EXPECT_EQ(checks, octal, 'A');
    LEARN_EXPECT_EQ(checks, universal, U'A');
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section10/brace_delimited_escape",
                                             "C++23 delimited escapes (__cpp_named_character_escapes capability gate)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/brace_delimited_escape", run>;

}  // namespace
