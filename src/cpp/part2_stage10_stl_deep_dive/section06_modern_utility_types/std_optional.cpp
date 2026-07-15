// Topic     : std::optional —— 可能有值 / 无值
// Doc       : 第2部分-阶段10 · 步骤 9.1
// cppreference: https://en.cppreference.com/cpp/utility/optional
//
// 要点: 空值语义; value/value_or; 禁止解引用空 optional(UB)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>

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
    std::cout << "=== [std_optional] ===\n";

    std::optional<int> a;
    assert(!a.has_value());
    assert(a == std::nullopt);
    assert(a.value_or(-1) == -1);

    a = 42;
    assert(a.has_value() && *a == 42 && a.value() == 42);

    a.reset();
    assert(!a);

    auto p = parse_positive("17");
    assert(p && *p == 17);
    assert(!parse_positive("-3"));
    assert(!parse_positive("x"));

    // emplace
    std::optional<std::string> name;
    name.emplace(5, 'x');
    assert(*name == "xxxxx");

    // 比较
    std::optional<int> x{3}, y{3}, z{4};
    assert(x == y && x < z);

    std::cout << "[optional] nullopt/value_or/parse/emplace OK\n";
    std::cout << "std_optional: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_optional", run>;

}  // namespace
