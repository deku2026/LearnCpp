// Topic    : std::error_code / error_condition / errc / category
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 6.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_error_code_category
// Topic id : part2/stage09/section05/std_error_code_category
// Refs     : https://en.cppreference.com/w/cpp/error/error_code
//            https://en.cppreference.com/w/cpp/error/error_condition
//            https://en.cppreference.com/w/cpp/error/errc
//            ISO [syserr]

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

namespace {

// 自定义 category 示例（教学最小版）
enum class AppErrc { Ok = 0, BadConfig = 1, Timeout = 2 };

class AppCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "app"; }
    std::string message(int ev) const override {
        switch (static_cast<AppErrc>(ev)) {
            case AppErrc::Ok:
                return "ok";
            case AppErrc::BadConfig:
                return "bad config";
            case AppErrc::Timeout:
                return "timeout";
        }
        return "unknown";
    }
};

const std::error_category& app_category() {
    static const AppCategory cat;
    return cat;
}

std::error_code make_error_code(AppErrc e) {
    return {static_cast<int>(e), app_category()};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_error_code_category] 主干：码 + category ===\n";
    {
        std::error_code ec = std::make_error_code(std::errc::no_such_file_or_directory);
        assert(ec);
        std::cout << "value=" << ec.value() << " category=" << ec.category().name() << " message=" << ec.message()
                  << '\n';

        // error_condition：可移植比较
        [[maybe_unused]] std::error_condition cond = ec.default_error_condition();
        assert(cond == std::errc::no_such_file_or_directory);
        std::cout << "condition portable match errc::no_such_file_or_directory\n";
    }

    std::cout << "=== 对抗：filesystem 的 error_code 重载（不抛）===\n";
    {
        std::error_code ec;
        const auto size = std::filesystem::file_size("definitely_missing_learncpp_stage09.bin", ec);
        (void)size;
        assert(ec);
        std::cout << "file_size missing -> " << ec.message() << '\n';
    }

    std::cout << "=== 对抗：自定义 category ===\n";
    {
        std::error_code ec = make_error_code(AppErrc::Timeout);
        assert(ec.category().name() == std::string{"app"});
        assert(ec.message() == "timeout");
        std::cout << "app category: " << ec.message() << '\n';
    }

    std::cout << "=== 专节：何时用 error_code ===\n";
    // 系统层 / 低层 API / 无异常 ABI 边界 / 与 C/OS 互通
    // 标准库常见“双接口”：抛异常版 + error_code 版
    std::cout << "use for OS/C ABI boundaries and dual APIs\n";

    std::cout << "[std_error_code_category] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_error_code_category", run>;

}  // namespace
