// Topic    : expected::and_then：下一步仍返回 expected（可失败链）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_and_then
// Topic id : part2/stage09/section04/expected_and_then
// Refs     : https://en.cppreference.com/w/cpp/utility/expected/and_then
//            P2505

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <string>

namespace {

enum class Error { Parse, Range };

std::expected<int, Error> parse(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::unexpected(Error::Parse);
    }
}

std::expected<int, Error> validate(int v) {
    if (v < 0 || v > 100) {
        return std::unexpected(Error::Range);
    }
    return v;
}

std::expected<int, Error> double_if_even(int v) {
    if (v % 2 != 0) {
        return std::unexpected(Error::Range);
    }
    return v * 2;
}

int g_validate_calls = 0;
int g_double_calls = 0;

std::expected<int, Error> validate_count(int v) {
    ++g_validate_calls;
    return validate(v);
}

std::expected<int, Error> double_count(int v) {
    ++g_double_calls;
    return double_if_even(v);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_and_then] 主干：可失败步骤链式 ===\n";
    {
        auto ok = parse("40").and_then(validate).and_then(double_if_even);
        assert(ok && *ok == 80);
        std::cout << "40 -> validate -> *2 = " << *ok << '\n';
    }

    std::cout << "=== 对抗：短路——前步失败后步不执行 ===\n";
    {
        g_validate_calls = g_double_calls = 0;
        auto bad = parse("nope").and_then(validate_count).and_then(double_count);
        assert(!bad && bad.error() == Error::Parse);
        assert(g_validate_calls == 0);
        assert(g_double_calls == 0);
        std::cout << "parse fail short-circuits; later calls=" << g_validate_calls << "/" << g_double_calls << '\n';
    }

    std::cout << "=== 对抗：中段失败同样短路 ===\n";
    {
        g_validate_calls = g_double_calls = 0;
        auto bad = parse("150").and_then(validate_count).and_then(double_count);
        assert(!bad && bad.error() == Error::Range);
        assert(g_validate_calls == 1);
        assert(g_double_calls == 0);
        std::cout << "validate fail; double not called\n";
    }

    std::cout << "=== 专节：and_then 的 f 必须返回 expected ===\n";
    // transform 的 f 返回普通值；and_then 的 f 返回 expected（下一步可失败）。
    std::cout << "and_then: f returns expected; transform: f returns plain T\n";

    std::cout << "[expected_and_then] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_and_then", run>;

}  // namespace
