// Topic    : expected::or_else：错误恢复 / 替换错误
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_or_else
// Topic id : part2/stage09/section04/expected_or_else
// Refs     : https://en.cppreference.com/w/cpp/utility/expected/or_else
//            P2505

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <string>

namespace {

std::expected<int, std::string> load_config(const std::string& key) {
    if (key == "port") {
        return 8080;
    }
    return std::unexpected("missing:" + key);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_or_else] 主干：错误时提供恢复 ===\n";
    {
        auto r = load_config("port").or_else([](const std::string&) {
            return std::expected<int, std::string>{80};  // 默认端口
        });
        assert(r && *r == 8080);  // 有值时 or_else 不调用
        std::cout << "port present -> " << *r << '\n';
    }

    {
        auto r = load_config("timeout").or_else([](const std::string& e) {
            std::cout << "  recovering from " << e << '\n';
            return std::expected<int, std::string>{30};
        });
        assert(r && *r == 30);
        std::cout << "timeout missing -> default " << *r << '\n';
    }

    std::cout << "=== 对抗：or_else 也可传播/改写错误 ===\n";
    {
        // or_else 的 F 必须返回 expected 特化（不能只 return unexpected）
        auto r = load_config("user").or_else(
            [](const std::string& e) { return std::unexpected<std::string>("fatal:" + e); });
        assert(!r && r.error() == "fatal:missing:user");
        std::cout << "rewritten error: " << r.error() << '\n';
    }

    std::cout << "=== 专节：有值时 or_else 短路（不调用）===\n";
    {
        int calls = 0;
        auto r = load_config("port").or_else([&](const std::string&) {
            ++calls;
            return std::expected<int, std::string>{0};
        });
        assert(r && *r == 8080 && calls == 0);
        std::cout << "value path skips or_else, calls=" << calls << '\n';
    }

    std::cout << "[expected_or_else] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_or_else", run>;

}  // namespace
