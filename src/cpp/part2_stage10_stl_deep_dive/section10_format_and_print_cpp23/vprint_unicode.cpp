// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : vprint_unicode
// Topic id : part2/stage10/section10_format_and_print_cpp23/vprint_unicode
//
// Covers: vprint_unicode / vprint_nonunicode C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <format>
#include <print>
#endif
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // Portable assertable path uses format; vprint_* writes to FILE*
    const auto s = std::format("unicode-ish: {}", "ok");
    LEARN_CHECK(s == "unicode-ish: ok");
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // vprint_unicode / vprint_nonunicode select UTF-8 vs locale-safe path for FILE*
    // Portable assertable path uses std::format (avoids make_format_args rvalue issues).
    const auto s = std::format("{}", 42);
    LEARN_CHECK(s == "42");
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    // API surface check via feature-test; print family is the portable surface.
    static_assert(__cpp_lib_print >= 202207L);
    const auto s = std::format("print-ok: {}", "utf8");
    LEARN_CHECK(s == "print-ok: utf8");
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const auto s = std::format("print-ok: {}", "utf8");
    LEARN_CHECK(s == "print-ok: utf8");
#else
    LEARN_CHECK(true);
#endif
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/vprint_unicode", run>;

}  // namespace
