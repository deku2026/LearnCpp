// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : rethrow_and_catch_all
// Topic id : part2/stage09/section01/rethrow_and_catch_all
//
// Covers: throw; rethrow, catch (...)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void layer_inner() {
    throw std::logic_error("inner");
}

void layer_middle() {
    try {
        layer_inner();
    } catch (...) {
        // log, then rethrow same exception object
        throw;
    }
}

void demo_basics() {
    try {
        layer_middle();
        assert(false);
    } catch (const std::logic_error& e) {
        assert(std::string{e.what()} == "inner");
    }
}

void demo_intermediate() {
    int hits = 0;
    try {
        try {
            throw 7;
        } catch (...) {
            ++hits;
            throw;
        }
    } catch (int v) {
        ++hits;
        assert(v == 7);
    }
    assert(hits == 2);
}

void demo_expert() {
    // catch-all then rethrow preserves type.
    try {
        try {
            throw std::runtime_error("x");
        } catch (...) {
            throw;
        }
    } catch (const std::runtime_error& e) {
        assert(std::string{e.what()} == "x");
    } catch (...) {
        assert(false);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/rethrow_and_catch_all", run>;

}  // namespace
