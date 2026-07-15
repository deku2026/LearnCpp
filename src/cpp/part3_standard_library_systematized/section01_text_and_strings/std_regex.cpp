// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.6 std::regex
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_regex
// Topic id : part3/section01/std_regex
//
// Refs:
//   https://en.cppreference.com/w/cpp/regex
//   https://en.cppreference.com/w/cpp/regex/regex_match
//   https://en.cppreference.com/w/cpp/regex/regex_search
//
// 要点: 能用但普遍慢; 高性能场景用 RE2/PCRE2。ECMAScript 语法默认。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <regex>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_regex ===\n";

    // match: 整串匹配
    const std::regex email(R"((\w+)@(\w+)\.(\w+))");
    std::cmatch m;
    [[maybe_unused]] const char* addr = "user@example.com";
    assert(std::regex_match(addr, m, email));
    assert(m.size() >= 4);
    assert(m[1] == "user");
    assert(m[2] == "example");
    assert(m[3] == "com");
    std::cout << "[intro] match " << m[0] << " local=" << m[1] << '\n';

    // search: 子串搜索
    const std::string text = "id=42; name=Ada";
    const std::regex id_re(R"(id=(\d+))");
    std::smatch sm;
    assert(std::regex_search(text, sm, id_re));
    assert(sm[1] == "42");
    std::cout << "[advanced] search id=" << sm[1] << '\n';

    // replace
    const std::string redacted = std::regex_replace(text, id_re, "id=**");
    assert(redacted.find("id=**") != std::string::npos);
    std::cout << "[advanced] replace -> " << redacted << '\n';

    // iterator: 找所有数字
    const std::regex num(R"(\d+)");
    const std::string multi = "a1 b23 c456";
    std::sregex_iterator it{multi.begin(), multi.end(), num};
    std::sregex_iterator end;
    [[maybe_unused]] int count = 0;
    for (; it != end; ++it) {
        ++count;
    }
    assert(count == 3);

    std::cout << "[expert] std::regex is convenient but often slow to compile "
                 "and match; prefer third-party for hot paths\n";
    std::cout << "std_regex: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_regex", run>;

}  // namespace
