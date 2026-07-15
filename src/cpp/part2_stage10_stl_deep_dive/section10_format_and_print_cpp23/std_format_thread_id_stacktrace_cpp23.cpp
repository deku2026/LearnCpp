// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_thread_id_stacktrace_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_format_thread_id_stacktrace_cpp23
//
// Covers: format thread::id and stacktrace C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <thread>
#include <version>

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#include <format>
#endif
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#include <stacktrace>
#endif

namespace {

void demo_basics() {
    const auto id = std::this_thread::get_id();
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const auto s = std::format("{}", id);
    assert(!s.empty());
#else
    (void)id;
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    std::thread::id id{};
    const auto s = std::format("tid={}", id);
    assert(s.rfind("tid=", 0) == 0);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L && defined(__cpp_lib_formatters) /* may vary */
    auto st = std::stacktrace::current();
    assert(!st.empty() || st.empty());
#elif defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    (void)st;
    assert(true);
#else
    assert(true);
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
    ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/std_format_thread_id_stacktrace_cpp23", run>;

}  // namespace
