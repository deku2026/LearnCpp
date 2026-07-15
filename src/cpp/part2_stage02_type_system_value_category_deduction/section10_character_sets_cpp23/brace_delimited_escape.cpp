// Topic    : C++23 花括号定界转义 \u{} / \x{} / \o{}
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 10.2
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : brace_delimited_escape
// Topic id : part2/stage02/section10/brace_delimited_escape
// Refs     : https://en.cppreference.com/cpp/language/escape
//            提案 P2290

#include "learn/topic_registry.hpp"

#include <cassert>
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

    std::cout << "=== [brace_delimited_escape] 入门：边界清晰的转义 ===\n";
    {
        // 旧：\x 贪婪吃掉后续十六进制；\u 必须正好 4 位
        [[maybe_unused]] const char* a = "\x41";    // 'A'
        [[maybe_unused]] const char* b = "\u00DF";  // ß

        // C++23：花括号定界，位数自由、边界明确
        [[maybe_unused]] const char* c = "\x{41}";   // 十六进制
        [[maybe_unused]] const char* d = "\u{DF}";   // Unicode 码点，不必凑 4 位
        [[maybe_unused]] const char* e = "\o{101}";  // 八进制（\o 为 C++23 引导）
        // 大码点用 \u{...}（Unicode 标量 → 源/执行编码下的码元序列）
        // 注意：\x{1F431} 是「单码元十六进制值」，对 char 会超出范围
        [[maybe_unused]] const char32_t cat32 = U'\u{1F431}';
        [[maybe_unused]] const char* f = "\u{1F431}";

        assert(std::string_view{a} == "A");
        assert(std::string_view{c} == "A");
        assert(std::string_view{b} == std::string_view{d});
        assert(std::string_view{e} == "A");  // octal 101 = 65 = 'A'
        assert(cat32 == U'\U0001F431');
        assert(!std::string_view{f}.empty());

        std::cout << "[intro] \\x{}/\\u{}/\\o{} delimit clearly\n";
    }

    std::cout << "=== 进阶：粘连问题对比；与后续十六进制字符 ===\n";
    {
        // 经典坑："\x41g" 是 A + g；但若写成 \x41a 会把 a 吃进十六进制
        [[maybe_unused]] const char* old_ok =
            "\x41"
            "g";                                       // 常用字符串拼接切断
        [[maybe_unused]] const char* neu = "\x{41}g";  // 花括号后 g 绝不会被吞
        assert(std::string_view{old_ok} == "Ag");
        assert(std::string_view{neu} == "Ag");

        // \u 旧式必须 4 位；\u{...} 可写 1–6 位有效码点
        [[maybe_unused]] const char* short_u = "\u{A}";  // 等同 U+000A 换行？实际是码点 U+000A
        assert(std::string_view{short_u} == "\n");

        [[maybe_unused]] const char32_t cat = U'\u{1F431}';  // 🐱
        assert(cat == U'\U0001F431');

        std::cout << "[advanced] braced form avoids hex run-on\n";
    }

    std::cout << "=== 专家：三种定界转义职责；特性宏 ===\n";
    {
        // \x{...} ：字节/码元的十六进制（与编码相关时要谨慎）
        // \u{...} ：Unicode 标量值
        // \o{...} ：八进制（新引导符，避免 \123 旧八进制的位数迷糊）
        [[maybe_unused]] const char* mix = "\x{48}\u{65}\o{154}\o{154}\x{6F}";  // Hello
        assert(std::string_view{mix} == "Hello");

#if defined(__cpp_delimited_escape_sequences)
        std::cout << "[expert] __cpp_delimited_escape_sequences=" << __cpp_delimited_escape_sequences << '\n';
#else
        std::cout << "[expert] __cpp_delimited_escape_sequences not defined\n";
#endif
        std::cout << "[expert] prefer braced escapes in new C++23 code\n";
    }

    std::cout << "[brace_delimited_escape] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/brace_delimited_escape", run>;

}  // namespace
