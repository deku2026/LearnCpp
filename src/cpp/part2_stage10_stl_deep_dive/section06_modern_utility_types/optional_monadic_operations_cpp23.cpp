// Topic     : optional 单子操作 and_then / transform / or_else (C++23)
// Doc       : 第2部分-阶段10 · 步骤 9.1
// cppreference: https://en.cppreference.com/cpp/utility/optional/and_then
//
// 要点: 管道式处理可选值; 空则短路。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace {

std::optional<int> to_int(std::string_view s) {
    try {
        std::size_t idx = 0;
        const int v = std::stoi(std::string{s}, &idx);
        if (idx != s.size()) return std::nullopt;
        return v;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<int> half_if_even(int x) {
    if (x % 2 == 0) return x / 2;
    return std::nullopt;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [optional_monadic_operations_cpp23] ===\n";

    // transform: optional<T> → optional<U>
    std::optional<int> n{21};
    [[maybe_unused]] auto doubled = n.transform([](int x) { return x * 2; });
    assert(doubled && *doubled == 42);
    assert(!std::optional<int>{}.transform([](int x) { return x; }));

    // and_then: 回调返回 optional
    [[maybe_unused]] auto r = to_int("10").and_then(half_if_even).transform([](int x) { return x + 1; });
    assert(r && *r == 6);  // 10/2+1

    [[maybe_unused]] auto fail = to_int("11").and_then(half_if_even);
    assert(!fail);

    // or_else: 空时提供后备 optional
    [[maybe_unused]] auto fallback = std::optional<int>{}.or_else([] { return std::optional<int>{7}; });
    assert(fallback == 7);

    [[maybe_unused]] auto keep = std::optional<int>{3}.or_else([] { return std::optional<int>{99}; });
    assert(keep == 3);

    // 管道: 解析 → 校验 → 映射
    auto pipeline = to_int("8")
                        .and_then(half_if_even)
                        .transform([](int x) { return std::string(static_cast<std::size_t>(x), '*'); })
                        .or_else([] { return std::optional<std::string>{"n/a"}; });
    assert(pipeline && *pipeline == "****");

    std::cout << "[monadic] and_then/transform/or_else pipeline OK\n";
    std::cout << "optional_monadic_operations_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/optional_monadic_operations_cpp23", run>;

}  // namespace
