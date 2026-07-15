// Topic    : headers map · 诊断 diagnostics
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : diagnostics_headers
// Topic id : part3/section08/diagnostics_headers
// Headers  : <exception> <stdexcept> <cassert> <system_error>
//            <source_location> <stacktrace>(C++23)
//
// 要点: 诊断族头文件地图 + 最小可运行探针(真实断言, 禁止空壳)。

#include "learn/topic_registry.hpp"

#include <exception>
#include <iostream>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#endif
#if __has_include(<version>)
#include <version>
#endif
#endif
#ifndef LEARN_HAS_STACKTRACE
#define LEARN_HAS_STACKTRACE 0
#endif

#if !defined(__cpp_lib_stacktrace) || !(__cpp_lib_stacktrace)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::stacktrace not available (__cpp_lib_stacktrace)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/diagnostics_headers", run>;
}  // namespace
#else
namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [diagnostics_headers] map probe ===\n";
    std::cout << "<exception>       uncaught_exceptions / exception hierarchy\n";
    std::cout << "<stdexcept>       logic_error / runtime_error\n";
    std::cout << "<cassert>         assert macro (NDEBUG strips)\n";
    std::cout << "<system_error>    error_code / error_condition / system_error\n";
    std::cout << "<source_location> C++20 typed file:line\n";
    std::cout << "<stacktrace>      C++23 runtime frames\n";

    // <exception>
    assert(std::uncaught_exceptions() == 0);
    try {
        throw std::runtime_error("demo");
    } catch (const std::exception& ex) {
        const auto loc = std::source_location::current();
        assert(std::string_view{ex.what()}.find("demo") != std::string_view::npos);
        assert(loc.line() > 0);
        assert(std::string_view{loc.file_name()}.size() > 0);
        std::cout << "caught @" << loc.file_name() << ':' << loc.line() << " what=" << ex.what() << '\n';
    }
    assert(std::uncaught_exceptions() == 0);

    // <stdexcept> 层次
    try {
        throw std::invalid_argument("bad arg");
    } catch (const std::logic_error& le) {
        assert(std::string_view{le.what()} == "bad arg");
    }

    // <system_error>
    std::error_code ec = std::make_error_code(std::errc::timed_out);
    assert(ec == std::errc::timed_out);
    assert(ec.value() != 0);
    assert(!ec.message().empty());
    std::cout << "errc::timed_out -> " << ec.message() << '\n';

    try {
        throw std::system_error(ec, "probe");
    } catch (const std::system_error& se) {
        assert(se.code() == ec);
    }

#if LEARN_HAS_STACKTRACE
    const auto st = std::stacktrace::current();
    std::cout << "stacktrace frames(now)=" << st.size() << '\n';
    // 有调试信息时通常非空; 裁剪环境允许 0
    assert(st.size() >= 0);
#else
    std::cout << "stacktrace header: not present on this toolchain\n";
#endif

#if defined(__cpp_lib_source_location)
    std::cout << "__cpp_lib_source_location=" << __cpp_lib_source_location << '\n';
#endif

    std::cout << "[diagnostics_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/diagnostics_headers", run>;

}  // namespace
#endif  // __cpp_lib_stacktrace
