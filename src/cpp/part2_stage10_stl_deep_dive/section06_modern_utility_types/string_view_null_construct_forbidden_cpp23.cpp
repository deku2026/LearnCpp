// Topic     : string_view 禁止 nullptr 构造 (C++23, P2166)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 6.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section06_modern_utility_types
// Item      : string_view_null_construct_forbidden_cpp23
// Topic id  : part2/stage10/section06/string_view_null_construct_forbidden_cpp23
// Refs      : https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view
//             P2166

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_null_construct_forbidden_cpp23] 入门：合法构造 ===\n";
    {
        [[maybe_unused]] std::string_view a{"hello"};
        std::string s = "owned";
        [[maybe_unused]] std::string_view b{s};
        std::string_view c{s.data(), s.size()};
        assert(a == "hello" && b == "owned" && c == "owned");
        std::cout << "literal/string/ptr+len OK\n";
    }

    std::cout << "=== 进阶：nullptr_t 构造 delete（编译期）===\n";
    {
        // ❌ C++23: 下列代码应无法编译
        // std::string_view bad = nullptr;
        // std::string_view bad2{nullptr};

        static_assert(!std::is_constructible_v<std::string_view, std::nullptr_t>);
        static_assert(!std::is_convertible_v<std::nullptr_t, std::string_view>);
        std::cout << "nullptr_t construct deleted OK\n";
    }

    std::cout << "=== 专家：空 view 正确写法 + 可空 C 字符串 ===\n";
    {
        // 空 view 用默认构造或 "" —— 不是 nullptr
        [[maybe_unused]] std::string_view empty{};
        assert(empty.empty());
        [[maybe_unused]] std::string_view empty2{""};
        assert(empty2.empty());

        // 从可能为空的 C 字符串: 必须先判空再构造
        [[maybe_unused]] const char* p = "ok";
        [[maybe_unused]] const char* q = nullptr;
        [[maybe_unused]] auto make_sv = [](const char* ptr) -> std::string_view {
            return ptr ? std::string_view{ptr} : std::string_view{};
        };
        assert(make_sv(p) == "ok");
        assert(make_sv(q).empty());

        // 历史：C++17 从 nullptr 构造是 UB；C++23 直接编译失败
        std::cout << "empty vs null C-string discipline OK\n";
    }

    std::cout << "[string_view_null_construct_forbidden_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06/string_view_null_construct_forbidden_cpp23", run>;

}  // namespace
