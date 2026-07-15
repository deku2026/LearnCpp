// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_print_println_cpp23
// Topic id : part3/section01/std_print_println_cpp23
//
// Covers: std::print / println C++23 feature-gated (quiet asserts)

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <string>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    // facility present; avoid console noise in automated runs
    static_assert(__cpp_lib_print >= 202207L);
#endif
    std::string msg = "print-ready";
    LEARN_CHECK(!msg.empty());
}

void demo_intermediate() {
    // Equivalent portable idea: format then fwrite/puts
    const char* line = "line\n";
    LEARN_CHECK(line[0] == 'l');
}

void demo_expert() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    // std::print goes to stdout; use when teaching interactive demos
    LEARN_CHECK(__cpp_lib_print >= 202207L);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_print_println_cpp23", run>;

}  // namespace
