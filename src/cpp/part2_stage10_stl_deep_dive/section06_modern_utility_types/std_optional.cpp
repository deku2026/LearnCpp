// Topic     : std::optional —— 可能有值 / 无值
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 9.1
// Stage     : part2_stage10_stl_deep_dive
// Section   : section06_modern_utility_types
// Item      : std_optional
// Topic id  : part2/stage10/section06/std_optional
// Refs      : https://en.cppreference.com/w/cpp/utility/optional

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

namespace {

std::optional<int> parse_positive(const std::string& s) {
    try {
        const int v = std::stoi(s);
        if (v > 0) return v;
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_optional] 入门：nullopt / has_value / value_or ===\n";
    {
        std::optional<int> a;
        assert(!a.has_value());
        assert(a == std::nullopt);
        assert(a.value_or(-1) == -1);
        assert(!a);

        a = 42;
        assert(a.has_value() && *a == 42 && a.value() == 42);

        a.reset();
        assert(!a);
        std::cout << "empty/engaged/reset OK\n";
    }

    std::cout << "=== 进阶：解析 API + emplace + 比较 ===\n";
    {
        auto p = parse_positive("17");
        assert(p && *p == 17);
        assert(!parse_positive("-3"));
        assert(!parse_positive("x"));

        std::optional<std::string> name;
        name.emplace(5, 'x');
        assert(*name == "xxxxx");

        std::optional<int> x{3}, y{3}, z{4};
        assert(x == y && x < z);
        assert(std::nullopt < x);  // 空小于任何有值（默认序）

        // in_place 构造
        std::optional<std::pair<int, int>> pr{std::in_place, 1, 2};
        assert(pr->first == 1 && pr->second == 2);
        std::cout << "parse/emplace/compare OK\n";
    }

    std::cout << "=== 专家：解引用空 optional 是 UB + value() 抛异常 ===\n";
    {
        std::optional<int> empty;
        // *empty; // ⚠️ UB：禁止
        bool threw = false;
        try {
            (void)empty.value();
        } catch (const std::bad_optional_access&) {
            threw = true;
        }
        assert(threw);

        // 安全模式：if / value_or / and_then(C++23 见 monadic 文件)
        int safe = empty.value_or(0);
        assert(safe == 0);

        // 返回 optional 表达「可能失败」，比 sentinel -1 更清晰
        auto maybe = parse_positive("0");
        assert(!maybe);
        std::cout << "bad_optional_access + safe access OK\n";
    }

    std::cout << "[std_optional] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_optional", run>;

}  // namespace
