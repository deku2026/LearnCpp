// Topic    : 何时用 expected vs 异常 vs optional vs error_code（选型验收）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.4 / 7
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_versus_exception_optional_error_code
// Topic id : part2/stage09/section04/expected_versus_exception_optional_error_code
// Refs     : C++ Core Guidelines E.*
//            https://en.cppreference.com/w/cpp/utility/expected
//            https://en.cppreference.com/w/cpp/utility/optional
//            https://en.cppreference.com/w/cpp/error/error_code

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>

namespace {

// (a) 异常：罕见、可跨多层、调用方可不处理
int divide_ex(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("div0");
    }
    return a / b;
}

// (b) expected：可恢复、必须处理、带原因
std::expected<int, std::string> divide_exp(int a, int b) {
    if (b == 0) {
        return std::unexpected(std::string{"div0"});
    }
    return a / b;
}

// (c) optional：只关心有/无，不关心原因
std::optional<int> divide_opt(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

// (d) error_code：系统/ABI/无异常边界
int divide_ec(int a, int b, std::error_code& ec) {
    if (b == 0) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return 0;
    }
    ec.clear();
    return a / b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_versus_...] 同一操作四种风格 ===\n";
    {
        try {
            assert(divide_ex(10, 2) == 5);
            (void)divide_ex(1, 0);
            assert(false);
        } catch (const std::invalid_argument& e) {
            std::cout << "exception: " << e.what() << '\n';
        }

        auto e = divide_exp(1, 0);
        assert(!e && e.error() == "div0");
        std::cout << "expected error: " << e.error() << '\n';

        auto o = divide_opt(1, 0);
        assert(!o);
        std::cout << "optional empty (no reason)\n";

        std::error_code ec;
        (void)divide_ec(1, 0, ec);
        assert(ec == std::errc::invalid_argument);
        std::cout << "error_code: " << ec.message() << '\n';
    }

    std::cout << "=== 选型四维度（决策表）===\n";
    // 1) 调用方是否必须处理？ 必须 → expected/optional；可不处理 → 异常
    // 2) 是否需要错误原因？ 需要 → expected/variant；只要有无 → optional
    // 3) 是否跨 ABI/无异常边界？ 是 → error_code/expected
    // 4) 失败是否常见？ 常见 → expected/optional；罕见 → 异常
    std::cout << "must-handle x need-reason x ABI x frequency\n";
    std::cout << "  parse/validate     -> expected\n";
    std::cout << "  map::find style    -> optional / iterator\n";
    std::cout << "  ctor invariant     -> exception\n";
    std::cout << "  filesystem dual    -> throw + error_code overloads\n";

    std::cout << "=== 成功路径对照 ===\n";
    {
        assert(divide_ex(8, 2) == 4);
        assert(*divide_exp(8, 2) == 4);
        assert(*divide_opt(8, 2) == 4);
        std::error_code ec;
        assert(divide_ec(8, 2, ec) == 4 && !ec);
        std::cout << "all four styles succeed for 8/2\n";
    }

    std::cout << "[expected_versus_exception_optional_error_code] all checks "
                 "passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage09/section04/expected_versus_exception_optional_error_code", run>;

}  // namespace
