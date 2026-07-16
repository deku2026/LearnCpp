// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : translation_unit_deep
// Topic id : part6/g/section02/translation_unit_deep
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/translation_unit_deep";

constexpr int internal_linkage_value = 7;
inline constexpr int header_style_inline_value = 11;

template <class T>
[[nodiscard]] constexpr T add_header_value(T value) {
    return value + static_cast<T>(header_style_inline_value);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr std::string_view source_file = __FILE__;
    LEARN_EXPECT(checks, !source_file.empty());
    LEARN_EXPECT_EQ(checks, internal_linkage_value, 7);
    LEARN_EXPECT_EQ(checks, add_header_value(4), 15);

    // A translation unit is the preprocessed source file: one primary source
    // plus included contents. Each TU is compiled independently before linking.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/translation_unit_deep", run>;

}  // namespace
