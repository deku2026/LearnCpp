// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : std_exception_hierarchy
// Topic id : part2/stage09/section01/std_exception_hierarchy
//
// Covers: std::exception hierarchy (logic_error, runtime_error, ...)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace {

const char* kind(const std::exception& e) {
    if (dynamic_cast<const std::invalid_argument*>(&e)) {
        return "invalid_argument";
    }
    if (dynamic_cast<const std::out_of_range*>(&e)) {
        return "out_of_range";
    }
    if (dynamic_cast<const std::logic_error*>(&e)) {
        return "logic_error";
    }
    if (dynamic_cast<const std::runtime_error*>(&e)) {
        return "runtime_error";
    }
    return "exception";
}

void demo_basics() {
    try {
        throw std::invalid_argument("bad");
    } catch (const std::exception& e) {
        assert(std::string{e.what()} == "bad");
        assert(std::string{kind(e)} == "invalid_argument");
    }
}

void demo_intermediate() {
    try {
        throw std::out_of_range("oor");
    } catch (const std::logic_error& e) {
        // out_of_range IS-A logic_error
        assert(std::string{kind(e)} == "out_of_range");
    }

    try {
        throw std::overflow_error("ovf");
    } catch (const std::runtime_error& e) {
        assert(std::string{e.what()} == "ovf");
    }
}

void demo_expert() {
    struct AppError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    try {
        throw AppError("app");
    } catch (const std::exception& e) {
        assert(std::string{e.what()} == "app");
        assert(dynamic_cast<const AppError*>(&e) != nullptr);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/std_exception_hierarchy", run>;

}  // namespace
