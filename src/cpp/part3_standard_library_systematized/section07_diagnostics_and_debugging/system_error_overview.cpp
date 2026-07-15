// Topic    : <system_error> error_code / errc / system_error
// Doc      : 第3部分-标准库系统化.md · 库 7.4
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : system_error_overview
// Topic id : part3/section07/system_error_overview
// Refs     : https://en.cppreference.com/w/cpp/error/error_code
//            https://en.cppreference.com/w/cpp/error/errc

#include "learn/topic_registry.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

namespace {

// 现代风格：用 error_code 而不是抛
std::error_code file_size_ec(const fs::path& p, std::uintmax_t& out) {
    std::error_code ec;
    out = fs::file_size(p, ec);
    return ec;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [system_error_overview] error_code 基础 ===\n";
    {
        std::error_code ok;
        assert(!ok);
        std::cout << "default error_code boolean=" << static_cast<bool>(ok) << '\n';

        std::error_code ec = std::make_error_code(std::errc::no_such_file_or_directory);
        assert(ec);
        assert(ec == std::errc::no_such_file_or_directory);
        std::cout << "errc message: " << ec.message() << " value=" << ec.value() << " category=" << ec.category().name()
                  << '\n';
    }

    std::cout << "=== error_condition 可移植比较 ===\n";
    {
        std::error_code ec = std::make_error_code(std::errc::invalid_argument);
        std::error_condition cond = ec.default_error_condition();
        assert(cond == std::errc::invalid_argument);
        std::cout << "condition: " << cond.message() << '\n';
    }

    std::cout << "=== system_error 异常 ===\n";
    {
        try {
            throw std::system_error(std::make_error_code(std::errc::permission_denied), "open config");
        } catch (const std::system_error& ex) {
            std::cout << "caught: " << ex.what() << " code=" << ex.code().value() << '\n';
            assert(ex.code() == std::errc::permission_denied);
        }
    }

    std::cout << "=== 与 filesystem 配合（不抛重载） ===\n";
    {
        std::uintmax_t sz = 0;
        const std::error_code ec = file_size_ec(fs::path("definitely_missing_learncpp_zzz.dat"), sz);
        assert(ec);
        std::cout << "file_size missing: " << ec.message() << '\n';
        (void)sz;
    }

    std::cout << "pair with std::expected<T, std::error_code> for modern APIs\n";
    std::cout << "[system_error_overview] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/system_error_overview", run>;

}  // namespace
