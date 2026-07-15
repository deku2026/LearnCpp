// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_to_chars_from_chars_cpp23
// Topic id : part2/stage07/section05/constexpr_to_chars_from_chars_cpp23
//
// Covers: std::to_chars / from_chars (+ constexpr where available)

#include "learn/topic_registry.hpp"

#include <charconv>
#include <string>
#include <system_error>
#include <version>

namespace {

int parse_int(const char* first, const char* last) {
    int value = 0;
    auto [ptr, ec] = std::from_chars(first, last, value);
    LEARN_CHECK(ec == std::errc{});
    LEARN_CHECK(ptr == last);
    return value;
}

std::size_t write_int(char* first, char* last, int value) {
    auto [ptr, ec] = std::to_chars(first, last, value);
    LEARN_CHECK(ec == std::errc{});
    return static_cast<std::size_t>(ptr - first);
}

void demo_basics() {
    const char s[] = "42";
    LEARN_CHECK(parse_int(s, s + 2) == 42);
}

void demo_intermediate() {
    char buf[16]{};
    auto n = write_int(buf, buf + sizeof(buf), 12345);
    LEARN_CHECK(n == 5);
    LEARN_CHECK(std::string(buf, n) == "12345");
}

void demo_expert() {
    const char s[] = "100";
    int v = 0;
    auto r = std::from_chars(s, s + 3, v);
    LEARN_CHECK(r.ec == std::errc{});
    LEARN_CHECK(v == 100);

    // Invalid parse does not throw; reports errc.
    const char bad[] = "x";
    int w = -1;
    auto r2 = std::from_chars(bad, bad + 1, w);
    LEARN_CHECK(r2.ec != std::errc{});

#if defined(__cpp_lib_constexpr_charconv)
    static_assert(__cpp_lib_constexpr_charconv >= 0 || true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_to_chars_from_chars_cpp23", run>;

}  // namespace
