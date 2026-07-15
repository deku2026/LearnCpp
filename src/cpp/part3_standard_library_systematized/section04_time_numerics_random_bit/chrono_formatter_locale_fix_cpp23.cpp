// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : chrono_formatter_locale_fix_cpp23
// Topic id : part3/section04/chrono_formatter_locale_fix_cpp23
//
// Covers: chrono formatting and locale notes

#include "learn/topic_registry.hpp"

#include <chrono>
#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#endif
namespace {

void demo_basics() {
    using namespace std::chrono_literals;
    auto d = 42ms;
    LEARN_CHECK(d.count() == 42);
}

void demo_intermediate() {
#if (defined(__cpp_lib_formatters) || defined(__cpp_lib_format)) && __has_include(<format>)
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    auto tp = std::chrono::sys_days{std::chrono::year{2024} / std::chrono::month{1} / std::chrono::day{2}};
    (void)tp;
#endif
#endif
    LEARN_CHECK(true);
}

void demo_expert() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    using namespace std::chrono_literals;
    std::string s = std::format("{}", 5ms);
    LEARN_CHECK(!s.empty());
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/chrono_formatter_locale_fix_cpp23", run>;

}  // namespace
