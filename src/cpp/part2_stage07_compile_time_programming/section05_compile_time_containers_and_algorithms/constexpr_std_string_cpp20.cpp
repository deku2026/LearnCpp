// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_std_string_cpp20
// Topic id : part2/stage07/section05/constexpr_std_string_cpp20
//
// Covers: constexpr std::string (C++20) with feature test

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <version>

namespace {

#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
constexpr std::size_t concat_len() {
    std::string s = "hello";
    s += " ";
    s += "world";
    return s.size();
}
#endif

void demo_basics() {
    std::string s = "hello";
    s += " world";
    assert(s == "hello world");
    assert(s.size() == 11);
}

void demo_intermediate() {
#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
    constexpr auto n = concat_len();
    static_assert(n == 11);
    assert(n == 11);
#else
    std::string s = "hello";
    s += " ";
    s += "world";
    assert(s.size() == 11);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
    constexpr auto c = []() constexpr {
        std::string s{"ab"};
        s.push_back('c');
        return s.size() == 3 && s[2] == 'c';
    }();
    static_assert(c);
    assert(c);
#else
    std::string s{"ab"};
    s.push_back('c');
    assert(s.size() == 3 && s[2] == 'c');
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_std_string_cpp20", run>;

}  // namespace
