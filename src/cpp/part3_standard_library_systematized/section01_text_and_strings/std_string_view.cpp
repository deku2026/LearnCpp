// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.2 string_view
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_view
// Topic id : part3/section01/std_string_view
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string_view
//
// 要点: 非拥有视图(指针+长度); 零拷贝 API 参数首选; 悬垂=最大坑。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

// API 参数用 string_view: 可接受 string / 字面量 / 其他 view, 无拷贝
[[maybe_unused]] std::size_t count_vowels(std::string_view sv) {
    std::size_t n = 0;
    for (char c : sv) {
        switch (c) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
            case 'A':
            case 'E':
            case 'I':
            case 'O':
            case 'U':
                ++n;
                break;
            default:
                break;
        }
    }
    return n;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_string_view ===\n";

    // -------------------------------------------------------------------------
    // §入门 — 从 string / 字面量构造
    // -------------------------------------------------------------------------
    std::string owned = "Hello, string_view";
    [[maybe_unused]] std::string_view sv = owned;
    assert(sv.data() == owned.data() || sv == owned);  // 通常共享缓冲
    assert(sv.size() == owned.size());
    assert(sv.substr(0, 5) == "Hello");

    [[maybe_unused]] std::string_view lit = "literal";
    assert(lit.size() == 7);
    assert(count_vowels(lit) == 3);  // i,e,a
    // "Hello, string_view" vowels: e,o + i + i,e = 5
    assert(count_vowels(owned) == 5);

    std::cout << "[intro] substr/count_vowels ok\n";

    // -------------------------------------------------------------------------
    // §进阶 — 操作与 remove_prefix/suffix
    // -------------------------------------------------------------------------
    std::string_view trim = "  pad  ";
    while (!trim.empty() && trim.front() == ' ') {
        trim.remove_prefix(1);
    }
    while (!trim.empty() && trim.back() == ' ') {
        trim.remove_suffix(1);
    }
    assert(trim == "pad");

    assert(std::string_view{"abc"}.starts_with('a'));
    assert(std::string_view{"abc"}.ends_with("bc"));
    assert(std::string_view{"abc"}.find('b') == 1);

    std::cout << "[advanced] remove_prefix/suffix + starts_with ok\n";

    // -------------------------------------------------------------------------
    // §专家 — 悬垂: 临时 string 上的 view 危险
    // -------------------------------------------------------------------------
    // 正确: view 的生命周期不得超过被指向数据
    std::string stable = "stable";
    [[maybe_unused]] std::string_view good = stable;
    assert(good == "stable");

    // 反模式(注释示意, 不执行悬垂读):
    //   std::string_view bad = std::string{"temp"}; // 临时销毁, bad 悬垂
    //   auto f() -> std::string_view { return std::string{"x"}; } // 返回悬垂

    // 从 C 数组: 注意字面量含隐式 '\0', string_view 不把 '\0' 算进 size
    const char arr[] = "xyz";
    std::string_view from_arr{arr};
    assert(from_arr.size() == 3);

    // 非空终止缓冲: string_view(ptr, len)
    const char raw[] = {'a', 'b', 'c', 'd'};
    std::string_view raw_sv{raw, 4};
    assert(raw_sv == "abcd");

    std::cout << "[expert] non-owning: never outlive the storage\n";
    std::cout << "std_string_view: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_view", run>;

}  // namespace
