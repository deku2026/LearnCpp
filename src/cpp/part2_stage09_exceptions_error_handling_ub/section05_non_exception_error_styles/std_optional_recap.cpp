// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_optional_recap
// Topic id : part2/stage09/section05/std_optional_recap
//
// Covers: std::optional as nullable success channel

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>
#include <string>

namespace {

std::optional<int> find_even(int x) {
    if (x % 2 == 0) {
        return x;
    }
    return std::nullopt;
}

void demo_basics() {
    auto a = find_even(4);
    assert(a.has_value());
    assert(*a == 4);
    assert(!find_even(3).has_value());
}

void demo_intermediate() {
    std::optional<std::string> s;
    assert(!s);
    s = "hi";
    assert(s.value() == "hi");
    s.reset();
    assert(s.value_or("def") == "def");
}

void demo_expert() {
    std::optional<int> o = 10;
    o = std::nullopt;
    assert(!o);
    o.emplace(7);
    assert(o.value() == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_optional_recap", run>;

}  // namespace
