// Topic     : std::string_view 基础：不拥有视图 / 零拷贝切片
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 6.1
// Stage     : part2_stage10_stl_deep_dive
// Section   : section06_modern_utility_types
// Item      : string_view_basics_cpp17
// Topic id  : part2/stage10/section06/string_view_basics_cpp17
// Refs      : https://en.cppreference.com/w/cpp/string/basic_string_view
//             P0220

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

namespace {

void process(std::string_view sv) {
    // 接受 string / 字面值 / const char* —— 均不拷贝
    assert(sv.data() != nullptr || sv.empty());
    (void)sv;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_basics_cpp17] 入门：指针+长度 / 多源参数 ===\n";
    {
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
        std::cout << "multi-source param + substr OK\n";
    }

    std::cout << "=== 进阶：constexpr / trim / starts_with ===\n";
    {
        constexpr std::string_view kHi = "hi";
        static_assert(kHi.size() == 2);
        static_assert(kHi[0] == 'h');

        std::string_view t = "  trim  ";
        while (!t.empty() && t.front() == ' ') t.remove_prefix(1);
        while (!t.empty() && t.back() == ' ') t.remove_suffix(1);
        assert(t == "trim");

        assert(std::string_view{"filename.cpp"}.ends_with(".cpp"));
        assert(std::string_view{"https://x"}.starts_with("https"));
        // vs const string&：字面值不会构造临时 string
        std::cout << "constexpr + trim + starts/ends_with OK\n";
    }

    std::cout << "=== 专家：不保证 '\\0' + 与 string 边界 ===\n";
    {
        // ⚠️ 切片后 data() 不能当 C 字符串
        [[maybe_unused]] std::string_view slice = std::string_view{"abcdef"}.substr(0, 3);  // "abc"
        assert(slice == "abc");
        assert(slice.size() == 3);
        // std::strlen(slice.data()) 可能越过 3 —— 危险

        // compare / find
        assert(std::string_view{"abc"}.compare("abc") == 0);
        assert(std::string_view{"hello"}.find("ll") == 2);

        // 空 view
        [[maybe_unused]] std::string_view empty{};
        assert(empty.empty() && empty.size() == 0);

        // 从 string 构造后 string 缩短：view 不跟踪 size（悬垂/越界风险）
        std::string owned = "abcdef";
        std::string_view view = owned;
        owned.resize(2);  // 可能使 view 越界（实现相关，教学：别让 view 活过修改）
        (void)view;
        // 纪律：修改 owned 后废弃旧 view
        view = owned;
        assert(view == "ab");
        std::cout << "no-NUL + compare/find + lifetime discipline OK\n";
    }

    std::cout << "[string_view_basics_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_basics_cpp17", run>;

}  // namespace
