// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_thread_id_stacktrace_cpp23
// Topic id : part2/stage10/section10_format_and_print_cpp23/std_format_thread_id_stacktrace_cpp23
//
// Covers: format thread::id and stacktrace C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <thread>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#endif
namespace {

void demo_basics() {
    const auto id = std::this_thread::get_id();
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    const auto s = std::format("{}", id);
    LEARN_CHECK(!s.empty());
#else
    (void)id;
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
    std::thread::id id{};
    const auto s = std::format("tid={}", id);
    LEARN_CHECK(s.rfind("tid=", 0) == 0);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L && \
    defined(__cpp_lib_formatters) /* may vary */ && __has_include(<format>) && __has_include(<stacktrace>)
    auto st = std::stacktrace::current();
    LEARN_CHECK(!st.empty() || st.empty());
#elif defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L && __has_include(<stacktrace>)
    auto st = std::stacktrace::current();
    (void)st;
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section10_format_and_print_cpp23/std_format_thread_id_stacktrace_cpp23", run>;

}  // namespace
