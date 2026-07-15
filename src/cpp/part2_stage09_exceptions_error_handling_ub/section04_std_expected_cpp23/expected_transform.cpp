// Topic    : expected::transform：纯映射（下一步不失败）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_transform
// Topic id : part2/stage09/section04/expected_transform
// Refs     : https://en.cppreference.com/w/cpp/utility/expected/transform
//            P2505

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
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_transform", run>;
}  // namespace
#else
#include <expected>
namespace {

std::expected<int, std::string> parse(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::unexpected(std::string{"parse"});
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_transform] 主干：值侧纯映射 ===\n";
    {
        auto r = parse("21").transform([](int v) { return v * 2; }).transform([](int v) { return std::to_string(v); });
        assert(r && *r == "42");
        std::cout << "21 *2 to_string -> " << *r << '\n';
    }

    std::cout << "=== 对抗：错误时 transform 跳过 ===\n";
    {
        int calls = 0;
        auto r = parse("xx").transform([&](int v) {
            ++calls;
            return v + 1;
        });
        assert(!r && r.error() == "parse");
        assert(calls == 0);
        std::cout << "error short-circuit, transform calls=" << calls << '\n';
    }

    std::cout << "=== 专节：与 and_then 选型 ===\n";
    // transform：映射不会失败（返回普通值，自动装进 expected）
    // and_then：下一步本身可能失败（返回 expected）
    auto only_map = parse("3").transform([](int v) { return v + 1; });
    assert(only_map && *only_map == 4);
    std::cout << "prefer transform for pure maps; and_then for fallible steps\n";

    std::cout << "=== 专家：链式类型演化（int → string）===\n";
    {
        auto r = parse("10")
                     .transform([](int v) { return v + 2; })               // expected<int,E>
                     .transform([](int v) { return std::to_string(v); });  // expected<string,E>
        assert(r && *r == "12");
        // 错误侧类型 E 在 transform 链上保持；改 E 用 transform_error
        std::cout << "type evolves on value side; E stable under transform\n";
    }

    std::cout << "[expected_transform] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_transform", run>;

}  // namespace
#endif  // __cpp_lib_expected
