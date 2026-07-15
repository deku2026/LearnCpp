// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : try_catch_throw_basics
// Topic id : part2/stage09/section01/try_catch_throw_basics
//
// Covers: throw, try, catch by value/ref, order of handlers

#include "learn/topic_registry.hpp"

#include <stdexcept>
#include <string>

namespace {

int parse_positive(int x) {
    if (x <= 0) {
        throw std::invalid_argument("non-positive");
    }
    return x;
}

void demo_basics() {
    try {
        parse_positive(-1);
        LEARN_CHECK(false);
    } catch (const std::invalid_argument& e) {
        LEARN_CHECK(std::string{e.what()}.find("non-positive") != std::string::npos);
    }
    LEARN_CHECK(parse_positive(3) == 3);
}

void demo_intermediate() {
    try {
        throw 42;
    } catch (int v) {
        LEARN_CHECK(v == 42);
    }

    try {
        throw std::string{"err"};
    } catch (const std::string& s) {
        LEARN_CHECK(s == "err");
    }
}

void demo_expert() {
    // More derived handlers first.
    try {
        throw std::out_of_range("oor");
    } catch (const std::out_of_range& e) {
        LEARN_CHECK(std::string{e.what()} == "oor");
    } catch (const std::exception& e) {
        (void)e;
        LEARN_CHECK(false);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/try_catch_throw_basics", run>;

}  // namespace
