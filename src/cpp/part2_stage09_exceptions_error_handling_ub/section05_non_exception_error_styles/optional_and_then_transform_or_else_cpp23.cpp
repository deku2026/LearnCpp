// Topic    : optional 单子操作 and_then/transform/or_else（C++23 验收）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 5.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : optional_and_then_transform_or_else_cpp23
// Topic id : part2/stage09/section05/optional_and_then_transform_or_else_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/optional
//            P0798

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>
#include <string>

namespace {

std::optional<int> parse_positive(const std::string& s) {
    try {
        const int v = std::stoi(s);
        return v > 0 ? std::optional<int>{v} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<int> half_if_even(int v) {
    if (v % 2 != 0) {
        return std::nullopt;
    }
    return v / 2;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [optional monadic] 主干：transform 链 ===\n";
    {
        auto result = parse_positive("21").transform([](int v) { return v * 2; }).transform([](int v) {
            return std::to_string(v);
        });
        assert(result && *result == "42");
        std::cout << "21 *2 -> " << *result << '\n';
    }

    std::cout << "=== 对抗：空值短路 ===\n";
    {
        int calls = 0;
        [[maybe_unused]] auto bad = parse_positive("-5").transform([&](int v) {
            ++calls;
            return v * 2;
        });
        assert(!bad);
        assert(calls == 0);
        assert(bad.value_or(-1) == -1);
        std::cout << "negative -> nullopt, transform skipped\n";
    }

    std::cout << "=== 对抗：and_then（下一步仍 optional）===\n";
    {
        [[maybe_unused]] auto r = parse_positive("10").and_then(half_if_even);
        assert(r && *r == 5);
        [[maybe_unused]] auto odd = parse_positive("9").and_then(half_if_even);
        assert(!odd);
        std::cout << "and_then half_if_even: 10->5, 9->nullopt\n";
    }

    std::cout << "=== 对抗：or_else 提供替代 ===\n";
    {
        auto r = parse_positive("0").or_else([] {
            return std::optional<int>{1};  // 默认
        });
        assert(r && *r == 1);
        std::cout << "or_else default=" << *r << '\n';
    }

    std::cout << "=== 专节：与 expected 单子同构 ===\n";
    // optional 短路传“空”；expected 短路传“错误”
    std::cout << "same monadic shape; optional has no error payload\n";

    std::cout << "[optional_and_then_transform_or_else_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage09/section05/optional_and_then_transform_or_else_cpp23", run>;

}  // namespace
