// Topic    : std::unexpected：显式构造错误通道
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : unexpected_construction
// Topic id : part2/stage09/section04/unexpected_construction
// Refs     : https://en.cppreference.com/w/cpp/utility/expected/unexpected
//            ISO [expected.un.general]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <string>
#include <utility>

namespace {

enum class Err : int { Ok = 0, NotFound = 1, Denied = 2 };

std::expected<int, Err> lookup(int id) {
    if (id < 0) {
        return std::unexpected(Err::Denied);
    }
    if (id == 0) {
        return std::unexpected(Err::NotFound);
    }
    return id * 10;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [unexpected_construction] 主干：unexpected 包装错误 ===\n";
    {
        std::expected<int, std::string> e1 = 42;  // 有值
        std::expected<int, std::string> e2 = std::unexpected("bad");
        assert(e1 && !e2);
        assert(e2.error() == "bad");

        // unexpected-tag 标签就地构造错误
        std::expected<int, std::string> e3 = std::unexpected(std::string{"in-place error"});
        assert(!e3 && e3.error() == "in-place error");
        std::cout << "unexpected wrapper builds the error channel\n";
    }

    std::cout << "=== 对抗：错误类型为枚举 ===\n";
    {
        auto a = lookup(3);
        assert(a && *a == 30);
        auto b = lookup(0);
        assert(!b && b.error() == Err::NotFound);
        auto c = lookup(-1);
        assert(!c && c.error() == Err::Denied);
        std::cout << "lookup errors: NotFound=" << int(Err::NotFound) << " Denied=" << int(Err::Denied) << '\n';
    }

    std::cout << "=== 专节：std::unexpected 可独立存在 ===\n";
    {
        std::unexpected u{std::string{"x"}};
        std::expected<double, std::string> e = std::move(u);
        assert(!e && e.error() == "x");
        std::cout << "unexpected is the explicit error vocabulary\n";
    }

    std::cout << "[unexpected_construction] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/unexpected_construction", run>;

}  // namespace
