// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : strong_guarantee
// Topic id : part2/stage09/section03/strong_guarantee
//
// Covers: strong guarantee — commit or rollback (as-if never called)

#include "learn/topic_registry.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct StrongBag {
    std::vector<std::string> items;

    void add_pair(const std::string& a, const std::string& b, bool fail_second) {
        // Build side effects on a temp, then commit.
        std::vector<std::string> next = items;
        next.push_back(a);
        if (fail_second) {
            throw std::runtime_error("fail before commit");
        }
        next.push_back(b);
        items.swap(next);  // commit
    }
};

void demo_basics() {
    StrongBag s;
    s.add_pair("x", "y", false);
    LEARN_CHECK(s.items.size() == 2);
    LEARN_CHECK(s.items[0] == "x");
}

void demo_intermediate() {
    StrongBag s;
    s.items.push_back("keep");
    try {
        s.add_pair("a", "b", true);
        LEARN_CHECK(false);
    } catch (...) {
        // Strong: original state preserved.
        LEARN_CHECK(s.items.size() == 1);
        LEARN_CHECK(s.items[0] == "keep");
    }
}

void demo_expert() {
    StrongBag s;
    s.add_pair("1", "2", false);
    try {
        s.add_pair("3", "4", true);
    } catch (...) {
    }
    LEARN_CHECK(s.items.size() == 2);
    LEARN_CHECK(s.items[1] == "2");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/strong_guarantee", run>;

}  // namespace
