// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_formatter_locale_fix_cpp23
// Topic id : part2/stage10/section11_other_stl/chrono_formatter_locale_fix_cpp23
//
// Covers: chrono format and parse C++20/23

#include "learn/topic_registry.hpp"

#include <chrono>
#include <string>
#include <version>

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    using namespace std::chrono;
    const auto d = 1500ms;
    const auto s = std::format("{}", d);
    LEARN_CHECK(!s.empty());
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    using namespace std::chrono;
    const year_month_day ymd{2024y / May / 1d};
    const auto s = std::format("{}", ymd);
    LEARN_CHECK(s.find("2024") != std::string::npos);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    using namespace std::chrono;
    std::chrono::sys_seconds tp{sys_days{2024y / January / 2d} + 3h + 4min + 5s};
    (void)tp;
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section11_other_stl/chrono_formatter_locale_fix_cpp23", run>;

}  // namespace
