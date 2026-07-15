// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.3 字节 ≠ 字符
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : byte_versus_char
// Topic id : part3/section01/byte_versus_char
//
// Refs:
//   https://en.cppreference.com/w/cpp/types/byte
//   https://en.cppreference.com/w/cpp/language/types
//
// 要点: char 常当“码元/字节容器”; std::byte 是“生字节”类型(不参与算术);
//       size() 数字节; Unicode 码点/字形要 ICU, 标准库不做。

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/byte_versus_char ===\n";

    // -------------------------------------------------------------------------
    // §入门 — char 三型 + 字节数
    // -------------------------------------------------------------------------
    static_assert(sizeof(char) == 1);
    static_assert(!std::is_same_v<char, signed char>);
    static_assert(!std::is_same_v<char, unsigned char>);

    // “héllo”: é 在 UTF-8 通常为 C3 A9 两字节 → size 常为 6
    const std::string text = "héllo";
    std::cout << "[intro] text=\"" << text << "\" size(bytes)=" << text.size() << '\n';
    assert(text.size() != 5 || text == "hello");  // 若源是纯 ASCII hello 则=5

    // 逐字节看(非“逐字符”)
    std::cout << "[intro] bytes:";
    for (unsigned char b : text) {
        std::cout << ' ' << std::hex << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // -------------------------------------------------------------------------
    // §进阶 — std::byte: 生字节, 禁止意外算术
    // -------------------------------------------------------------------------
    std::byte b{0xAB};
    b <<= 1;  // 仅允许位移/位运算族
    assert(std::to_integer<int>(b) == 0x56);

    // unsigned char 常用于对象表示窥视; std::byte 语义更清晰
    const std::uint32_t x = 0x01020304;
    const auto* raw = reinterpret_cast<const std::byte*>(&x);
    // 打印首字节(端序相关, 只演示类型用法)
    const int first = std::to_integer<int>(raw[0]);
    std::cout << "[advanced] first byte of 0x01020304 = 0x" << std::hex << first << std::dec << " (endian-dependent)\n";
    (void)first;

    // -------------------------------------------------------------------------
    // §专家 — 字节 / 码元 / 码点 / 字形
    // -------------------------------------------------------------------------
    // 字节: 存储单元
    // 码元(code unit): char8_t/char16_t/char32_t 序列中的一个元素
    // 码点(code point): Unicode 标量值(一个抽象字符编号)
    // 字形(grapheme): 用户感知的一个“字符”, 可能多码点组合
    // 标准库 string 只懂字节/码元层面的连续存储, 不做规范化/分词。
    std::cout << "[expert] byte != code unit != code point != grapheme\n";
    std::cout << "byte_versus_char: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/byte_versus_char", run>;

}  // namespace
