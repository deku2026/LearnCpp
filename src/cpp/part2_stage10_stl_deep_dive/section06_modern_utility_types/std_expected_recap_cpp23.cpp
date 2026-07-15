// Topic     : std::expected 回顾 —— 值或错误 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 9.1
// cppreference: https://en.cppreference.com/cpp/utility/expected
//
// 要点: 判别联合语义; and_then/transform/or_else 铁路式; vs optional/异常。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <version>

#if !defined(__cpp_lib_expected) || !(__cpp_lib_expected) || !__has_include(<expected>)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::expected not available (__cpp_lib_expected)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_expected_recap_cpp23", run>;
}  // namespace
#else
#include <expected>
namespace {

enum class Err { Empty, NotNumber, Negative };

std::expected<int, Err> parse_nonneg(std::string_view s) {
    if (s.empty()) return std::unexpected(Err::Empty);
    try {
        std::size_t idx = 0;
        const int v = std::stoi(std::string{s}, &idx);
        if (idx != s.size()) return std::unexpected(Err::NotNumber);
        if (v < 0) return std::unexpected(Err::Negative);
        return v;
    } catch (...) {
        return std::unexpected(Err::NotNumber);
    }
}

std::expected<int, Err> reciprocal_int(int x) {
    if (x == 0) return std::unexpected(Err::NotNumber);
    return 100 / x;
}

const char* err_msg(Err e) {
    switch (e) {
        case Err::Empty:
            return "empty";
        case Err::NotNumber:
            return "not number";
        case Err::Negative:
            return "negative";
    }
    return "?";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_expected_recap_cpp23] ===\n";

    [[maybe_unused]] auto ok = parse_nonneg("25");
    assert(ok.has_value() && *ok == 25);
    assert(ok.value_or(-1) == 25);

    [[maybe_unused]] auto bad = parse_nonneg("-1");
    assert(!bad && bad.error() == Err::Negative);

    // 铁路式
    [[maybe_unused]] auto r = parse_nonneg("20").and_then(reciprocal_int).transform([](int x) { return x * 2; });
    assert(r && *r == 10);  // (100/20)*2

    [[maybe_unused]] auto r2 = parse_nonneg("0").and_then(reciprocal_int);
    assert(!r2);

    // or_else 恢复
    [[maybe_unused]] auto recovered = parse_nonneg("x").or_else([](Err) { return std::expected<int, Err>{0}; });
    assert(recovered && *recovered == 0);

    // transform_error
    [[maybe_unused]] auto mapped = parse_nonneg("").transform_error([](Err e) { return err_msg(e); });
    assert(!mapped && mapped.error() == std::string_view{"empty"});

    std::cout << "[expected] value/error + monadic rail OK\n";
    std::cout << "std_expected_recap_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_expected_recap_cpp23", run>;

}  // namespace
#endif  // __cpp_lib_expected
