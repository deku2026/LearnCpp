// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 optional
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : optional_overview
// Topic id : part3/section03/optional_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/optional
//   and_then / transform / or_else (C++23 monadic ops)

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>
#include <string>

namespace {

std::optional<int> parse_positive(const std::string& s) {
    try {
        const int v = std::stoi(s);
        if (v > 0) {
            return v;
        }
    } catch (...) {
    }
    return std::nullopt;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/optional_overview ===\n";

    [[maybe_unused]] std::optional<int> empty;
    assert(!empty.has_value());
    assert(empty.value_or(-1) == -1);

    [[maybe_unused]] std::optional<int> answer{42};
    assert(answer && *answer == 42);
    assert(answer.value() == 42);

    // 就地构造
    std::optional<std::string> name{std::in_place, "Ada"};
    assert(name->size() == 3);

    std::cout << "[intro] has_value / value_or ok\n";

    // C++23 单子操作
    [[maybe_unused]] auto doubled = parse_positive("21").transform([](int x) { return x * 2; });
    assert(doubled == 42);

    [[maybe_unused]] auto chain =
        parse_positive("10")
            .and_then([](int x) -> std::optional<int> { return x > 0 ? std::optional{x + 1} : std::nullopt; })
            .or_else([] { return std::optional<int>{0}; });
    assert(chain == 11);

    [[maybe_unused]] auto fallback = parse_positive("-3").or_else([] { return std::optional<int>{0}; });
    assert(fallback == 0);

    std::cout << "[advanced] transform/and_then/or_else ok\n";
    std::cout << "[expert] optional = maybe value; not for errors with payload "
                 "(use expected)\n";
    std::cout << "optional_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/optional_overview", run>;

}  // namespace
