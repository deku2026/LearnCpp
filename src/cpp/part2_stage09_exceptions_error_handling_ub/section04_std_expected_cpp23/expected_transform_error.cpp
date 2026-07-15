// Topic    : expected::transform_error：转换错误类型 / 统一错误通道
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.3 / 坑 #16
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_transform_error
// Topic id : part2/stage09/section04/expected_transform_error
// Refs     : https://en.cppreference.com/w/cpp/utility/expected/transform_error
//            P2549

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
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_transform_error", run>;
}  // namespace
#else
#include <expected>
namespace {

enum class IoError { Open, Read };
enum class AppError { Io, Parse, Logic };

std::expected<std::string, IoError> read_file(bool ok) {
    if (!ok) {
        return std::unexpected(IoError::Open);
    }
    return std::string{"42"};
}

std::expected<int, AppError> parse_number(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::unexpected(AppError::Parse);
    }
}

const char* app_err_name(AppError e) {
    switch (e) {
        case AppError::Io:
            return "Io";
        case AppError::Parse:
            return "Parse";
        case AppError::Logic:
            return "Logic";
    }
    return "?";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_transform_error] 主干：统一错误类型 ===\n";
    {
        // IoError → AppError，才能与后续 AppError 管道衔接
        auto r = read_file(true).transform_error([](IoError) { return AppError::Io; }).and_then(parse_number);
        assert(r && *r == 42);
        std::cout << "success pipeline value=" << *r << '\n';
    }

    {
        auto r = read_file(false).transform_error([](IoError) { return AppError::Io; }).and_then(parse_number);
        assert(!r && r.error() == AppError::Io);
        std::cout << "io fail -> " << app_err_name(r.error()) << '\n';
    }

    std::cout << "=== 对抗：有值时 transform_error 不调用 ===\n";
    {
        int calls = 0;
        std::expected<int, std::string> ok = 1;
        auto r = ok.transform_error([&](const std::string& e) {
            ++calls;
            return e + "!";
        });
        assert(r && *r == 1 && calls == 0);
        std::cout << "value path skips transform_error\n";
    }

    std::cout << "=== 专节：错误侧映射 vs or_else ===\n";
    // transform_error：错误 → 新错误值（类型可变更），仍是错误通道
    // or_else：错误 → 整个 expected（可恢复成值）
    std::expected<int, int> e = std::unexpected(7);
    auto mapped = e.transform_error([](int x) { return x * 10; });
    assert(!mapped && mapped.error() == 70);
    std::cout << "transform_error 7 -> " << mapped.error() << '\n';

    std::cout << "[expected_transform_error] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_transform_error", run>;

}  // namespace
#endif  // __cpp_lib_expected
