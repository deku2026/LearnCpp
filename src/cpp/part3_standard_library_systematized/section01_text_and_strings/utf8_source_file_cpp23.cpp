// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.3 UTF-8 源文件 (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : utf8_source_file_cpp23
// Topic id : part3/section01/utf8_source_file_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/charset
//   https://en.cppreference.com/w/cpp/language/string_literal
//   P2295 / phase translation — UTF-8 source files recognized by the standard
//
// 要点: C++23 明确源文件可用 UTF-8; 字面量中的非 ASCII 与 u8 前缀。
//       本文件本身应按 UTF-8 保存(无 BOM 或带 BOM 均可, 视工具链)。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/utf8_source_file_cpp23 ===\n";

    // 源中直接写 UTF-8 文本(文件须以 UTF-8 编码保存)
    const std::string greeting = "你好, C++";
    assert(!greeting.empty());
    std::cout << "[intro] greeting bytes=" << greeting.size() << " text=" << greeting << '\n';

    // u8 字面量 → char8_t 序列; 与 char 字符串的互操作需显式转换策略
    const char8_t* u8hi = u8"π≈3.14";
    assert(u8hi[0] != 0);
    // 把 u8 当字节看: 转 string_view 需经 unsigned char 或 memcpy 风格
    const auto* bytes = reinterpret_cast<const char*>(u8hi);
    std::string_view as_bytes{bytes};
    assert(as_bytes.find("3.14") != std::string_view::npos);

    // 转义形式不依赖“源文件里有没有那个字形”
    const std::string via_escape = "\u4F60\u597D";  // 你好
    assert(via_escape == "你好");

    std::cout << "[advanced] Unicode escapes portable even if editor fonts differ\n";
    std::cout << "[expert] C++23 standardizes UTF-8 as a source file encoding; "
                 "still configure compiler (/utf-8 on MSVC) for consistency\n";
    std::cout << "utf8_source_file_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/utf8_source_file_cpp23", run>;

}  // namespace
