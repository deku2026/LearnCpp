// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 expected (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : expected_overview_cpp23
// Topic id : part3/section03/expected_overview_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/expected
//   Feature-test: __cpp_lib_expected >= 202202L
//
// 要点: 值或错误; 铁路式 and_then/transform/or_else; 替代“输出参数+错误码”。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<expected>)
#include <expected>
#endif
#endif

namespace {

enum class ParseErr { empty, not_int, negative };

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

std::expected<int, ParseErr> parse_nonneg(const std::string& s) {
    if (s.empty()) {
        return std::unexpected{ParseErr::empty};
    }
    try {
        const int v = std::stoi(s);
        if (v < 0) {
            return std::unexpected{ParseErr::negative};
        }
        return v;
    } catch (...) {
        return std::unexpected{ParseErr::not_int};
    }
}

#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/expected_overview_cpp23 ===\n";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto ok = parse_nonneg("7");
    assert(ok.has_value() && *ok == 7);

    auto bad = parse_nonneg("-1");
    assert(!bad.has_value());
    assert(bad.error() == ParseErr::negative);

    // 铁路式: 成功继续, 失败短路
    auto pipe =
        parse_nonneg("10").transform([](int x) { return x * 3; }).and_then([](int x) -> std::expected<int, ParseErr> {
            if (x > 100) {
                return std::unexpected{ParseErr::negative};  // reuse as demo
            }
            return x;
        });
    assert(pipe.has_value() && *pipe == 30);

    auto recovered = parse_nonneg("x").or_else([](ParseErr) -> std::expected<int, ParseErr> { return 0; });
    assert(recovered == 0);

    std::cout << "[intro] expected value/error + monadic ops ok\n";
    std::cout << "[expert] __cpp_lib_expected=" << __cpp_lib_expected << '\n';
#else
    std::cout << "[intro] std::expected unavailable on this STL\n";
    (void)ParseErr::empty;
#endif

    std::cout << "expected_overview_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/expected_overview_cpp23", run>;

}  // namespace
