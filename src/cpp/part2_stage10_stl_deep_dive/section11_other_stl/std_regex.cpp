// Topic    : <regex> 匹配 / 搜索 / 替换(功能全但偏慢)
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_regex
// Topic id : part2/stage10/section11/std_regex
// Refs     : https://en.cppreference.com/w/cpp/regex
//            https://en.cppreference.com/w/cpp/regex/regex_match
//            https://en.cppreference.com/w/cpp/regex/regex_search

#include "learn/topic_registry.hpp"

#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_regex] match / search / replace ===\n";

    // ① regex_match: 整串匹配
    {
        const std::regex email{R"((\w+)@(\w+)\.(\w+))"};
        const std::string s = "alice@example.com";
        std::smatch m;
        assert(std::regex_match(s, m, email));
        assert(m.size() == 4);
        assert(m[0] == "alice@example.com");
        assert(m[1] == "alice");
        assert(m[2] == "example");
        assert(m[3] == "com");
        std::cout << "  match email user=" << m[1] << " domain=" << m[2] << '\n';
    }

    // ② regex_search: 子串搜索
    {
        const std::regex re{R"(\d{3}-\d{4})"};
        const std::string text = "call 555-1234 now";
        std::smatch m;
        assert(std::regex_search(text, m, re));
        assert(m.str() == "555-1234");
        assert(m.prefix().str() == "call ");
        std::cout << "  search phone=" << m.str() << '\n';
    }

    // ③ regex_replace
    {
        const std::regex re{R"(\s+)"};
        const std::string in = "a   b\tc";
        const std::string out = std::regex_replace(in, re, "_");
        assert(out == "a_b_c");
        std::cout << "  replace ws → " << out << '\n';
    }

    // ④ sregex_iterator: 找所有匹配
    {
        const std::regex re{R"(\w+)"};
        const std::string text = "one two three";
        std::vector<std::string> words;
        for (std::sregex_iterator it{text.begin(), text.end(), re}, end{}; it != end; ++it) {
            words.push_back(it->str());
        }
        assert((words == std::vector<std::string>{"one", "two", "three"}));
        std::cout << "  iterator words=" << words.size() << '\n';
    }

    // ⑤ ECMAScript 语法是默认; 原始字符串 R"(...)" 避免双转义
    {
        // 匹配 C++ 风格标识符
        const std::regex id{R"([A-Za-z_][A-Za-z0-9_]*)"};
        assert(std::regex_match("foo_1", id));
        assert(!std::regex_match("1foo", id));
        std::cout << "  identifier pattern OK\n";
    }

    // 教学: <regex> 便于正确性/工具脚本; 热路径/大文本常换 PCRE/hyperscan/手写
    std::cout << "[std_regex] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/std_regex", run>;

}  // namespace
