// Topic    : C++23 UTF-8 可移植源文件编码 + 字符类型回顾
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 10.3 / 10.4
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section10_character_sets_cpp23
// Item     : utf8_source_file_encoding
// Topic id : part2/stage02/section10/utf8_source_file_encoding
// Refs     : https://en.cppreference.com/cpp/language/string_literal
//            https://en.cppreference.com/cpp/language/charset
//            提案 P2295 (UTF-8 source file encoding)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

// 本文件假定以 UTF-8 保存（无 BOM 或带 BOM 均可，视工具链）。
// C++23 P2295：实现应把 UTF-8 源文件解释为可移植的源字符集输入，
// 减轻 Windows GBK 默认与 Unix UTF-8 默认不一致导致的乱码/编译差异。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [utf8_source_file_encoding] 入门：源中直接写 Unicode ===\n";
    {
        // 注释与字符串均可含非 ASCII（源文件 UTF-8）
        // 注释：π、中文、emoji 意图
        const char* pi_in_comment_demo = "π";
        const char* hello_zh = "你好";
        assert(std::string_view{pi_in_comment_demo}.size() >= 2);  // UTF-8 多字节
        assert(std::string_view{hello_zh}.size() == 6);            // 3+3 bytes in UTF-8

        // 推荐：意图是 UTF-8 码元序列时用 u8 字面值（元素 char8_t）
        const char8_t* u8zh = u8"你好";
        assert(u8zh[0] != 0);
        std::cout << "[intro] ordinary string holds UTF-8 bytes if source is UTF-8\n";
        std::cout << "  hello_zh bytes=" << std::string_view{hello_zh}.size() << '\n';
    }

    std::cout << "=== 进阶：字符家族与字面值前缀 ===\n";
    {
        // char      —— 普通执行字符集，符号性实现定义
        // char8_t   —— UTF-8 码元（C++20）
        // char16_t  —— UTF-16 码元
        // char32_t  —— UTF-32 码元
        // wchar_t   —— 宽字符，宽度实现定义（可移植性差）

        const char* ordinary = "A";
        const char8_t* u8s = u8"A";
        const char16_t* u16 = u"A";
        const char32_t* u32 = U"A";
        const wchar_t* wide = L"A";

        static_assert(std::is_same_v<decltype(u8s), const char8_t*>);
        static_assert(std::is_same_v<decltype(u16), const char16_t*>);
        static_assert(std::is_same_v<decltype(u32), const char32_t*>);

        assert(ordinary[0] == 'A');
        assert(u8s[0] == u8'A');
        assert(u16[0] == u'A');
        assert(u32[0] == U'A');
        assert(wide[0] == L'A');

        // 与转义配合（回顾步骤 10.1/10.2）
        const char8_t* mixed = u8"\u{03C0}";  // π
        assert(mixed[0] != 0);

        std::cout << "[advanced] prefer u8 for portable UTF-8 text data\n";
    }

    std::cout << "=== 专家：工程建议；Qt 对照；特性宏 ===\n";
    {
        // 工程清单：
        // 1) 源文件统一 UTF-8（编辑器 + .editorconfig / repo 规范）
        // 2) MSVC 可加 /utf-8；clang-cl 同样建议显式 UTF-8
        // 3) 跨平台字符串数据优先 u8"" / char8_t 或明确转码层
        // 4) 不要假设 char 的符号性；字节用 unsigned char / std::byte
        //
        // 🔶 Qt：QString 内部 UTF-16；源 UTF-8 + QStringLiteral/u8 转 QString
        //    可减少「源编码各异」导致的中文乱码。

        std::u8string_view sv = u8"C++23";
        assert(sv.size() == 5);

#if defined(__cpp_unicode_literals)
        std::cout << "[expert] __cpp_unicode_literals=" << __cpp_unicode_literals << '\n';
#endif
#if defined(__cpp_char8_t)
        std::cout << "[expert] __cpp_char8_t=" << __cpp_char8_t << '\n';
#endif
        // P2295 相关宏名随实现，以能否稳定编译 UTF-8 源 + 非 ASCII 字面值为准
        std::cout << "[expert] save sources as UTF-8; verify on target toolchain\n";
    }

    std::cout << "[utf8_source_file_encoding] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section10/utf8_source_file_encoding", run>;

}  // namespace
