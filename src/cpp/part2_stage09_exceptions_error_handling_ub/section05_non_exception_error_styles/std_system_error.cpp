// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_system_error
// Topic id : part2/stage09/section05/std_system_error
//
// Covers: std::system_error exception wrapping error_code

#include "learn/topic_registry.hpp"

#include <string>
#include <system_error>

namespace {

void may_fail(bool fail) {
    if (fail) {
        throw std::system_error(std::make_error_code(std::errc::permission_denied), "open");
    }
}

void demo_basics() {
    try {
        may_fail(true);
        LEARN_CHECK(false);
    } catch (const std::system_error& e) {
        LEARN_CHECK(e.code() == std::errc::permission_denied);
        LEARN_CHECK(std::string{e.what()}.find("open") != std::string::npos);
    }
}

void demo_intermediate() {
    may_fail(false);
    std::error_code ec = std::make_error_code(std::errc::timed_out);
    std::system_error se(ec, "wait");
    LEARN_CHECK(se.code() == std::errc::timed_out);
}

void demo_expert() {
    try {
        throw std::system_error(std::make_error_code(std::errc::io_error), "io");
    } catch (const std::runtime_error& e) {
        // system_error inherits runtime_error
        LEARN_CHECK(std::string{e.what()}.size() > 0);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_system_error", run>;

}  // namespace
