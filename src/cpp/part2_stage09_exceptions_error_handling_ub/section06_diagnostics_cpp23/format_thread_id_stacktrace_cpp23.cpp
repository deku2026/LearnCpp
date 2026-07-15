// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : format_thread_id_stacktrace_cpp23
// Topic id : part2/stage09/section06/format_thread_id_stacktrace_cpp23
//
// Covers: formatting thread id (+ stacktrace when available) for diagnostics

#include "learn/topic_registry.hpp"

#include <sstream>
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

std::string thread_tag() {
    std::ostringstream os;
    os << std::this_thread::get_id();
    return os.str();
}

void demo_basics() {
    auto id = thread_tag();
    LEARN_CHECK(!id.empty());
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    auto s = std::format("tid={}", thread_tag());
    LEARN_CHECK(s.find("tid=") == 0);
#else
    auto s = std::string{"tid="} + thread_tag();
    LEARN_CHECK(s.find("tid=") == 0);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    auto msg = thread_tag() + " frames=" + std::to_string(st.size());
    LEARN_CHECK(msg.find("frames=") != std::string::npos);
#else
    auto msg = thread_tag() + " frames=n/a";
    LEARN_CHECK(msg.find("frames=") != std::string::npos);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/format_thread_id_stacktrace_cpp23", run>;

}  // namespace
