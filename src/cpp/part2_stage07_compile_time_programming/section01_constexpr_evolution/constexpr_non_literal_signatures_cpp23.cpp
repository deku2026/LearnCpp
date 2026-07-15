// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_non_literal_signatures_cpp23
// Topic id : part2/stage07/section01/constexpr_non_literal_signatures_cpp23
//
// Covers: constexpr functions with non-literal params when not constant-evaluated

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <version>

namespace {

// Always OK: literal types in signature for constexpr use.
constexpr int len_of_array(const char* s, int n) {
    int i = 0;
    while (i < n && s[i] != '\0') {
        ++i;
    }
    return i;
}

// C++23 relaxes constexpr function parameter/return constraints in some cases.
// Portable demo: overload that is constexpr only for literal path.
constexpr int twice_int(int x) {
    return x * 2;
}

int twice_string_size(const std::string& s) {
    return static_cast<int>(s.size()) * 2;
}

void demo_basics() {
    static_assert(twice_int(21) == 42);
    assert(twice_int(21) == 42);
}

void demo_intermediate() {
    static_assert(len_of_array("hello", 16) == 5);
    assert(len_of_array("ab", 8) == 2);
}

void demo_expert() {
    // Non-literal std::string used at runtime only (safe portable teaching).
    std::string s = "abcd";
    assert(twice_string_size(s) == 8);

#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
    // When available, string itself can participate in constexpr contexts elsewhere.
    static_assert(__cpp_lib_constexpr_string >= 201907L);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_non_literal_signatures_cpp23", run>;

}  // namespace
