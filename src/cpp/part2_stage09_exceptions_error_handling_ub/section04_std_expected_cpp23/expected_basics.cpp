// Topic    : std::expected<T,E> 基础：值或错误，二选一（C++23）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.1 / 4.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_basics
// Topic id : part2/stage09/section04/expected_basics
// Refs     : https://en.cppreference.com/w/cpp/utility/expected
//            P0323
//            ISO [expected]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <version>

#if !defined(__cpp_lib_expected) || !(__cpp_lib_expected) || !__has_include(<expected>)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::expected not available (__cpp_lib_expected)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_basics", run>;
}  // namespace
#else
#include <expected>
namespace {

std::expected<int, std::string> parse_int(const std::string& s) {
    try {
        std::size_t idx = 0;
        const int v = std::stoi(s, &idx);
        if (idx != s.size()) {
            return std::unexpected("trailing junk: " + s);
        }
        return v;
    } catch (...) {
        return std::unexpected("cannot parse: " + s);
    }
}

std::expected<void, std::string> save_ok(bool ok) {
    if (!ok) {
        return std::unexpected(std::string{"disk full"});
    }
    return {};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_basics] 主干：有值 / 有错误 ===\n";
    {
        auto r1 = parse_int("42");
        assert(r1.has_value());
        assert(static_cast<bool>(r1));
        assert(*r1 == 42);
        std::cout << "parse 42 -> value " << *r1 << '\n';

        auto r2 = parse_int("abc");
        assert(!r2);
        assert(r2.error().find("cannot parse") != std::string::npos);
        std::cout << "parse abc -> error: " << r2.error() << '\n';
    }

    std::cout << "=== 对抗：value_or / emplace / void expected ===\n";
    {
        assert(parse_int("7").value_or(-1) == 7);
        assert(parse_int("x").value_or(-1) == -1);

        std::expected<int, std::string> e = 1;
        e.emplace(99);
        assert(*e == 99);

        auto ok = save_ok(true);
        assert(ok.has_value());
        auto bad = save_ok(false);
        assert(!bad && bad.error() == "disk full");
        std::cout << "expected<void,E> for side-effect ops\n";
    }

    std::cout << "=== 专节：就地存储、无动态分配心智 ===\n";
    // expected 要么 T 要么 E，总是其一；值与错误 in-place，无堆。
    std::expected<int, std::string> e = std::unexpected("bad");
    assert(!e);
    std::cout << "in-place value-or-error vocabulary type\n";

    std::cout << "[expected_basics] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_basics", run>;

}  // namespace
#endif  // __cpp_lib_expected
