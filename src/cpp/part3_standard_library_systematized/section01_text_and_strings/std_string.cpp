// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.1 std::string
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string
// Topic id : part3/section01/std_string
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string
//   https://en.cppreference.com/w/cpp/string/basic_string/capacity
//
// 要点: string 是字节序列(非“字符序列”); size()=字节数; SSO 短串不堆分配;
//       capacity/reserve; starts_with/ends_with (C++20); 与 QString 心智不同。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_string ===\n";

    // -------------------------------------------------------------------------
    // §入门 — 字节序列 + SSO 直觉
    // -------------------------------------------------------------------------
    std::string s = "Hello, World";
    assert(s.size() == 12);
    assert(s == "Hello, World");
    assert(s[0] == 'H');
    assert(s.front() == 'H' && s.back() == 'd');

    // size() 是字节数: UTF-8 多字节字符会让“看起来 5 个字”的串 size>5
    std::string utf8 = "héllo";  // é 通常 2 字节 UTF-8
    std::cout << "[intro] \"héllo\" size(bytes)=" << utf8.size() << " (often 6, not 5)\n";
    assert(utf8.size() >= 5);

    // SSO: 短串 capacity 通常 >= size, 且对象本身可容纳短数据
    std::string short_s = "SSO";
    std::cout << "[intro] short sizeof(string)=" << sizeof(std::string) << " size=" << short_s.size()
              << " capacity=" << short_s.capacity() << '\n';
    assert(short_s.capacity() >= short_s.size());

    // -------------------------------------------------------------------------
    // §进阶 — 修改 / 查找 / C++20 starts_with·ends_with
    // -------------------------------------------------------------------------
    s.append("!");
    assert(s == "Hello, World!");
    s.insert(0, "Say: ");
    assert(s.starts_with("Say: "));
    assert(s.ends_with("!"));

    const auto pos = s.find("World");
    assert(pos != std::string::npos);
    s.replace(pos, 5, "C++");
    assert(s.find("C++") != std::string::npos);

    // reserve: 预留容量减少再分配
    std::string buf;
    buf.reserve(64);
    assert(buf.capacity() >= 64);
    for (int i = 0; i < 10; ++i) {
        buf.push_back(static_cast<char>('0' + i));
    }
    assert(buf == "0123456789");

    // string_view 零拷贝视图(不拥有)
    [[maybe_unused]] std::string_view sv = s;
    assert(sv.size() == s.size());
    assert(std::string{sv.substr(0, 4)} == "Say:");

    std::cout << "[advanced] mutate/find/starts_with ok: " << s << '\n';

    // -------------------------------------------------------------------------
    // §专家 — c_str 保证 / 迭代器失效直觉 / 比较是字节序
    // -------------------------------------------------------------------------
    // c_str()/data() 保证以 '\0' 结尾的连续缓冲(C++11 起 data 也保证)
    const char* c = s.c_str();
    assert(c[s.size()] == '\0');
    (void)c;

    // 比较: 按 char 的字典序(字节比较), 不是 locale/Unicode 排序
    assert(std::string{"apple"} < std::string{"banana"});
    assert(std::string{"Zoo"} < std::string{"apple"});  // 'Z' < 'a' 在 ASCII

    // 与 size_type / npos
    static_assert(std::string::npos == static_cast<std::string::size_type>(-1));

    std::cout << "[expert] c_str null-terminated; compare is byte-wise\n";
    std::cout << "std_string: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string", run>;

}  // namespace
