// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : c_compat_headers
// Topic id : part3/section08/c_compat_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section08/c_compat_headers";

constexpr std::array<std::string_view, 29> headers{
    "assert.h",  "ctype.h",  "errno.h",     "fenv.h",        "float.h",   "inttypes.h", "limits.h", "locale.h",
    "math.h",    "setjmp.h", "signal.h",    "stdarg.h",      "stddef.h",  "stdint.h",   "stdio.h",  "stdlib.h",
    "string.h",  "time.h",   "uchar.h",     "wchar.h",       "wctype.h",  "complex.h",  "iso646.h", "stdalign.h",
    "stdbool.h", "tgmath.h", "stdatomic.h", "stdnoreturn.h", "threads.h",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, headers.size(), 29U);
    LEARN_EXPECT_EQ(checks, std::strlen("C bridge"), 8U);
    LEARN_EXPECT_EQ(checks, std::abs(-7), 7);
    LEARN_EXPECT(checks, std::string_view{headers.front()} == "assert.h");
    // Some names are compatibility/empty headers or not part of a given revision; __has_include decides availability.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/c_compat_headers", run>;

}  // namespace
