// Topic    : std::system_error：携带 error_code 的异常
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 6.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_system_error
// Topic id : part2/stage09/section05/std_system_error
// Refs     : https://en.cppreference.com/w/cpp/error/system_error
//            ISO [syserr.syserr]

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

namespace {

void open_or_throw(const std::filesystem::path& p) {
    std::error_code ec;
    if (!std::filesystem::exists(p, ec) || ec) {
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "open_or_throw");
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_system_error] 主干：error_code → 异常 ===\n";
    {
        try {
            throw std::system_error(std::make_error_code(std::errc::permission_denied), "access");
        } catch (const std::system_error& e) {
            assert(e.code() == std::errc::permission_denied);
            std::cout << "what=" << e.what() << " code=" << e.code().value() << " msg=" << e.code().message() << '\n';
        }
    }

    std::cout << "=== 对抗：捕获为 std::exception 仍可用 ===\n";
    {
        try {
            open_or_throw("no_such_stage09_path");
            assert(false);
        } catch (const std::exception& e) {
            // system_error 派生自 runtime_error → exception
            std::cout << "as std::exception: " << e.what() << '\n';
        }
    }

    std::cout << "=== 专节：双接口桥接心智 ===\n";
    // 底层 API 写 error_code；需要跨层时 throw system_error(ec, what)
    // 或在边界把异常转回 error_code（Qt/C ABI）
    std::error_code ec = std::make_error_code(std::errc::timed_out);
    try {
        if (ec) {
            throw std::system_error(ec, "timeout bridge");
        }
    } catch (const std::system_error& e) {
        assert(e.code() == std::errc::timed_out);
        std::cout << "bridged: " << e.code().message() << '\n';
    }

    std::cout << "[std_system_error] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_system_error", run>;

}  // namespace
