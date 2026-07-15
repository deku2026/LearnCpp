// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_calendar_timezones_cpp20
// Topic id : part2/stage10/section11_other_stl/chrono_calendar_timezones_cpp20
//
// Covers: chrono calendar and time zones C++20

#include "learn/topic_registry.hpp"

#include <chrono>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    using namespace std::chrono;
    const year_month_day ymd{2024y / March / 15d};
    LEARN_CHECK(ymd.ok());
    LEARN_CHECK(static_cast<int>(ymd.year()) == 2024);
    LEARN_CHECK(unsigned{ymd.month()} == 3);
    LEARN_CHECK(unsigned{ymd.day()} == 15);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    using namespace std::chrono;
    const auto ymd = 2024y / 2 / 29;
    LEARN_CHECK(ymd.ok());
    const auto bad = 2023y / 2 / 29;
    LEARN_CHECK(!bad.ok());
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    using namespace std::chrono;
    try {
        const auto* tz = current_zone();
        LEARN_CHECK(tz != nullptr);
        const auto now = system_clock::now();
        const auto local = tz->to_local(now);
        (void)local;
    } catch (...) {
        // timezone db may be unavailable on some systems
        LEARN_CHECK(true);
    }
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
    ::learn::topic<"part2/stage10/section11_other_stl/chrono_calendar_timezones_cpp20", run>;

}  // namespace
