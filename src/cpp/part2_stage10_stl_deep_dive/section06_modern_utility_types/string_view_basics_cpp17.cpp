// Topic     : std::string_view 基础：不拥有视图 / 零拷贝切片
// Doc       : 第2部分-阶段10 · 步骤 6.1
// cppreference: https://en.cppreference.com/cpp/string/basic_string_view
//
// 要点: 指针+长度; 函数参数取代 const string&; substr 零拷贝; 不保证 '\0'。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

namespace {

void process(std::string_view sv) {
    // 接受 string / 字面值 / const char* —— 均不拷贝
    assert(!sv.empty() || sv.data() != nullptr || sv.size() == 0);
    (void)sv;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_basics_cpp17] ===\n";

    std::string s = "hello world";
    process(s);
    process("literal");
    process(s.c_str());

    std::string_view sv = s;
    assert(sv.size() == 11);
    assert(sv.substr(0, 5) == "hello");
    sv = sv.substr(6);  // "world" —— 只改指针与长度
    assert(sv == "world");
    assert(s == "hello world");  // 底层未改

    // 字面值直接绑定(静态存储, 安全)
    constexpr std::string_view kHi = "hi";
    static_assert(kHi.size() == 2);

    // remove_prefix / remove_suffix
    std::string_view t = "  trim  ";
    while (!t.empty() && t.front() == ' ') t.remove_prefix(1);
    while (!t.empty() && t.back() == ' ') t.remove_suffix(1);
    assert(t == "trim");

    // ⚠️ 不保证以 '\0' 结尾 —— 切片后 data() 不能当 C 字符串
    std::string_view slice = std::string_view{"abcdef"}.substr(0, 3);  // "abc"
    assert(slice == "abc");
    // std::strlen(slice.data()) 可能越过 3 —— 危险; 用 sv 接口

    // starts_with / ends_with (C++20)
    assert(std::string_view{"filename.cpp"}.ends_with(".cpp"));
    assert(std::string_view{"https://x"}.starts_with("https"));

    std::cout << "[string_view] param/substr/trim/starts_with OK\n";
    std::cout << "string_view_basics_cpp17: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_basics_cpp17", run>;

}  // namespace
