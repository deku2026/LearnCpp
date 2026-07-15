// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.3.1 clang-format
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : clang_format_setup
// Topic id : part4/section03/clang_format_setup
//
// 要点: .clang-format 统一风格; CI dry-run --Werror; 消除格式争论。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct FormatStyle {
    std::string based_on{"Google"};
    int indent_width{4};
    int column_limit{100};
    std::string pointer_alignment{"Left"};  // int* p
    bool sort_includes{true};
};

// 教学: 指针贴左 vs 贴右
std::string format_pointer(std::string_view type, std::string_view name, std::string_view align) {
    if (align == "Left") {
        return std::string(type) + "* " + std::string(name);
    }
    return std::string(type) + " *" + std::string(name);
}

// 模拟 SortIncludes: 按字典序
std::vector<std::string> sort_includes(std::vector<std::string> inc) {
    std::sort(inc.begin(), inc.end());
    return inc;
}

// dry-run: 格式化后是否变化
bool needs_format(std::string_view before, std::string_view after) {
    return before != after;
}

// 折叠连续空格(极简「格式化」)
std::string collapse_spaces(std::string_view in) {
    std::string out;
    bool sp = false;
    for (char c : in) {
        if (c == ' ' || c == '\t') {
            if (!sp) {
                out.push_back(' ');
            }
            sp = true;
        } else {
            sp = false;
            out.push_back(c);
        }
    }
    return out;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== clang_format_setup ===\n";

    FormatStyle style{};
    assert(style.indent_width == 4);
    assert(style.column_limit == 100);
    assert(format_pointer("int", "p", "Left") == "int* p");
    assert(format_pointer("int", "p", "Right") == "int *p");
    std::cout << "  PointerAlignment Left => " << format_pointer("int", "p", "Left") << '\n';

    auto sorted = sort_includes({"#include <vector>", "#include <algorithm>", "#include \"a.hpp\""});
    assert(sorted.front().find("a.hpp") != std::string::npos || sorted[0] < sorted[1]);
    assert(sorted[0] <= sorted[1] && sorted[1] <= sorted[2]);

    const std::string messy = "int    x  =  1;";
    const std::string clean = collapse_spaces(messy);
    assert(needs_format(messy, clean));
    assert(clean == "int x = 1;");
    std::cout << "  dry-run would fail on messy input\n";

    // CI: clang-format --dry-run --Werror
    [[maybe_unused]] const int dry_run_exit = needs_format(messy, clean) ? 1 : 0;
    assert(dry_run_exit == 1);

    // 本仓库风格: 教学示例倾向 4 空格(与文档 Google Based + IndentWidth 4)
    assert(style.based_on == "Google");
    std::cout << "  BasedOnStyle=" << style.based_on << " IndentWidth=" << style.indent_width << '\n';

    std::cout << "clang_format_setup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/clang_format_setup", run>;

}  // namespace
