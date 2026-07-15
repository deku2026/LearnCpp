// Topic    : headers map · 诊断 diagnostics
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : diagnostics_headers
// Topic id : part3/section08/diagnostics_headers
// Headers  : <exception> <stdexcept> <cassert> <system_error>
//            <source_location> <stacktrace>(C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <exception>
#include <iostream>
#include <source_location>
#include <stdexcept>
#include <string>
#include <system_error>

#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#endif
#endif
#ifndef LEARN_HAS_STACKTRACE
#define LEARN_HAS_STACKTRACE 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [diagnostics_headers] map probe ===\n";
    std::cout << "<exception>       uncaught_exceptions=" << std::uncaught_exceptions() << '\n';
    std::cout << "<stdexcept>       logic/runtime hierarchy\n";
    std::cout << "<cassert>         assert macro (this file uses it)\n";
    std::cout << "<system_error>    error_code / system_error\n";
    std::cout << "<source_location> C++20 typed file:line\n";
    std::cout << "<stacktrace>      C++23 runtime frames\n";

    try {
        throw std::runtime_error("demo");
    } catch (const std::exception& ex) {
        const auto loc = std::source_location::current();
        std::cout << "caught @" << loc.file_name() << ':' << loc.line() << " what=" << ex.what() << '\n';
    }

    std::error_code ec = std::make_error_code(std::errc::timed_out);
    assert(ec == std::errc::timed_out);
    std::cout << "errc::timed_out -> " << ec.message() << '\n';

#if LEARN_HAS_STACKTRACE
    std::cout << "stacktrace frames(now)=" << std::stacktrace::current().size() << '\n';
#else
    std::cout << "stacktrace header: not present on this toolchain\n";
#endif

    assert(true);
    std::cout << "[diagnostics_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/diagnostics_headers", run>;

}  // namespace
