// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_print_for_diagnostics_cpp23
// Topic id : part2/stage09/section06/std_print_for_diagnostics_cpp23
//
// Covers: std::print / format-style diagnostics (C++23) with portable fallback

#include "learn/topic_registry.hpp"

#include <sstream>
#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#if __has_include(<print>)
#include <print>
#endif
#endif
namespace {

std::string diag_line(int code, const std::string& msg) {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    return std::format("code={} msg={}", code, msg);
#else
    std::ostringstream os;
    os << "code=" << code << " msg=" << msg;
    return os.str();
#endif
}

void demo_basics() {
    auto s = diag_line(1, "ok");
    LEARN_CHECK(s.find("code=1") != std::string::npos);
    LEARN_CHECK(s.find("ok") != std::string::npos);
}

void demo_intermediate() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L && __has_include(<print>)
    // std::print writes to stdout; we still assert format helper works.
    auto s = diag_line(42, "answer");
    LEARN_CHECK(s.find("42") != std::string::npos);
#else
    auto s = diag_line(42, "answer");
    LEARN_CHECK(s == "code=42 msg=answer");
#endif
}

void demo_expert() {
    // Prefer structured diagnostic strings over ad-hoc concatenation.
    auto a = diag_line(0, "x");
    auto b = diag_line(0, "x");
    LEARN_CHECK(a == b);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/std_print_for_diagnostics_cpp23", run>;

}  // namespace
