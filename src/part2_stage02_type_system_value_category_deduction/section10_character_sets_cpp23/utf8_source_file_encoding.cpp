// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : utf8_source_file_encoding
// Topic id : part2/stage02/section10/utf8_source_file_encoding
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
    constexpr char8_t text[] = u8"π";
    using LiteralType = std::remove_reference_t<decltype((u8"π"))>;
    static_assert(std::is_same_v<std::remove_extent_t<LiteralType>, const char8_t>);
    static_assert(std::extent_v<LiteralType> == 3);

    learn::ExampleChecks checks{"part2/stage02/section10/utf8_source_file_encoding"};
    LEARN_EXPECT_EQ(checks, sizeof(text), std::size_t{3});
    LEARN_EXPECT_EQ(checks, static_cast<unsigned char>(text[0]), 0xCFU);
    LEARN_EXPECT_EQ(checks, static_cast<unsigned char>(text[1]), 0x80U);
    LEARN_EXPECT_EQ(checks, text[2], char8_t{});
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section10/utf8_source_file_encoding", "__cpp_char8_t");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/utf8_source_file_encoding", run>;

}  // namespace
