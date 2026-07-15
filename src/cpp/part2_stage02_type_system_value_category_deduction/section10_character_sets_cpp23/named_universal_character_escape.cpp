// Topic    : C++23 命名通用字符转义 \N{...}
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 10.1
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : named_universal_character_escape
// Topic id : part2/stage02/section10/named_universal_character_escape
// Refs     : https://en.cppreference.com/cpp/language/escape
//            提案 P2071

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [named_universal_character_escape] 入门：用 Unicode 名写字符 ===\n";
    {
        // 不必死记码点：用官方 Unicode 名称
        const char* beta = "\N{GREEK SMALL LETTER BETA}";  // β
        const char* infinity = "\N{INFINITY}";             // ∞
        // 猫脸等 emoji 名称以编译器 Unicode 数据库为准
        [[maybe_unused]] const char32_t beta_c = U'\N{GREEK SMALL LETTER BETA}';

        assert(std::string_view{beta} == std::string_view{"\u03B2"});
        assert(std::string_view{infinity} == std::string_view{"\u221E"});
        assert(beta_c == U'\u03B2');

        std::cout << "[intro] beta=" << beta << " infinity=" << infinity << '\n';
    }

    std::cout << "=== 进阶：与 u8/u/U 字面值配合；自解释源码 ===\n";
    {
        [[maybe_unused]] const char8_t* u8beta = u8"\N{GREEK SMALL LETTER BETA}";
        [[maybe_unused]] const char16_t* u16beta = u"\N{GREEK SMALL LETTER BETA}";
        [[maybe_unused]] const char32_t* u32beta = U"\N{GREEK SMALL LETTER BETA}";

        assert(u8beta[0] != 0);
        assert(u16beta[0] == u'\u03B2');
        assert(u32beta[0] == U'\u03B2');

        // 对比：\u03B2 要查表；\N{...} 读源码即懂
        std::string msg = "value \N{GREEK CAPITAL LETTER PI} ~ 3.14";
        assert(msg.find("\u03A0") != std::string::npos);

        std::cout << "[advanced] works in u8/u/U/ordinary strings\n";
    }

    std::cout << "=== 专家：名称必须精确；特性探测 ===\n";
    {
        // 错误名称 → 编译错误（注释示意）：
        // const char* bad = "\N{NOT A REAL CHARACTER NAME}";

        // 空格与连字符依 Unicode 名称规范（通常大写单词空格分隔）
        [[maybe_unused]] const char* nbsp_name = "\N{NO-BREAK SPACE}";
        assert(std::string_view{nbsp_name}.size() >= 1);

#if defined(__cpp_named_character_escapes)
        std::cout << "[expert] __cpp_named_character_escapes=" << __cpp_named_character_escapes << '\n';
#else
        std::cout << "[expert] __cpp_named_character_escapes not defined\n";
#endif
        std::cout << "[expert] prefer \\N{NAME} over magic code points when supported\n";
    }

    std::cout << "[named_universal_character_escape] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/named_universal_character_escape", run>;

}  // namespace
