// Topic    : 铁路式管道：解析→校验→计算（验收点）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : railway_oriented_pipeline
// Topic id : part2/stage09/section04/railway_oriented_pipeline
// Refs     : https://en.cppreference.com/w/cpp/utility/expected
//            P0323 P2505
//            C++ Stories: std::expected monadic ops

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
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/railway_oriented_pipeline", run>;
}  // namespace
#else
#include <expected>
namespace {

enum class Error { ParseError, OutOfRange, ComputeError };

std::expected<int, Error> parse(const std::string& s) {
    try {
        std::size_t idx = 0;
        const int v = std::stoi(s, &idx);
        if (idx != s.size()) {
            return std::unexpected(Error::ParseError);
        }
        return v;
    } catch (...) {
        return std::unexpected(Error::ParseError);
    }
}

std::expected<int, Error> validate(int v) {
    if (v < 0 || v > 100) {
        return std::unexpected(Error::OutOfRange);
    }
    return v;
}

int compute(int v) {
    return v * v;
}  // 纯映射，不会失败

// 可选：可能失败的计算
std::expected<int, Error> compute_checked(int v) {
    if (v == 13) {
        return std::unexpected(Error::ComputeError);
    }
    return v * v;
}

const char* err_name(Error e) {
    switch (e) {
        case Error::ParseError:
            return "ParseError";
        case Error::OutOfRange:
            return "OutOfRange";
        case Error::ComputeError:
            return "ComputeError";
    }
    return "?";
}

std::expected<int, Error> pipeline(const std::string& s) {
    // ⭐ 无嵌套 if：解析 → 校验 → 计算；任一步失败即短路
    return parse(s).and_then(validate).transform(compute);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [railway_oriented_pipeline] 主干：成功路径 ===\n";
    {
        auto result = pipeline("50");
        assert(result && *result == 2500);
        std::cout << "50 -> " << *result << '\n';
    }

    std::cout << "=== 对抗：校验失败短路（compute 不跑）===\n";
    {
        int compute_calls = 0;
        auto bad = parse("150").and_then(validate).transform([&](int v) {
            ++compute_calls;
            return compute(v);
        });
        assert(!bad && bad.error() == Error::OutOfRange);
        assert(compute_calls == 0);
        std::cout << "150 fails: " << err_name(bad.error()) << " compute_calls=" << compute_calls << '\n';
    }

    std::cout << "=== 对抗：解析失败 ===\n";
    {
        auto bad = pipeline("abc");
        assert(!bad && bad.error() == Error::ParseError);
        std::cout << "abc fails: " << err_name(bad.error()) << '\n';
    }

    std::cout << "=== 对抗：全 and_then 版本 + or_else 兜底 ===\n";
    {
        auto r = parse("13").and_then(validate).and_then(compute_checked).or_else([](Error) {
            // 业务兜底：给默认结果
            return std::expected<int, Error>{0};
        });
        assert(r && *r == 0);
        std::cout << "13 compute error recovered to " << *r << '\n';
    }

    std::cout << "=== 专节：对比嵌套 if ===\n";
    // 传统：auto p=parse; if(!p)return; auto v=validate(*p); if(!v)return; ...
    // 铁路：错误处理集中在链尾，主流程一目了然。
    std::cout << "railway: happy path is one chain; errors collect at the end\n";

    std::cout << "[railway_oriented_pipeline] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/railway_oriented_pipeline", run>;

}  // namespace
#endif  // __cpp_lib_expected
