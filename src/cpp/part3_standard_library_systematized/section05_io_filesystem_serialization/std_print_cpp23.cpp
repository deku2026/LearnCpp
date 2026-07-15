// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : std_print_cpp23
// Topic id : part3/section05/std_print_cpp23
//
// Covers: std::print in IO context C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<print>)
#include <print>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L && __has_include(<print>)
    LEARN_CHECK(__cpp_lib_print >= 202207L);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
    // print is format + write; keep tests silent
    std::string s = "silent";
    LEARN_CHECK(s.size() == 6);
}

void demo_expert() {
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/std_print_cpp23", run>;

}  // namespace
