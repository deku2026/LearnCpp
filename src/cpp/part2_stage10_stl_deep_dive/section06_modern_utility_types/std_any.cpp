// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_any
// Topic id : part2/stage10/section06_modern_utility_types/std_any
//
// Covers: std::any type-erased value

#include "learn/topic_registry.hpp"

#include <any>
#include <cassert>
#include <string>

namespace {

void demo_basics() {
    std::any a = 42;
    assert(a.has_value());
    assert(std::any_cast<int>(a) == 42);
    a = std::string{"hi"};
    assert(std::any_cast<std::string>(a) == "hi");
}

void demo_intermediate() {
    std::any a;
    assert(!a.has_value());
    a = 3.14;
    assert(a.type() == typeid(double));
    a.reset();
    assert(!a.has_value());
}

void demo_expert() {
    std::any a = 10;
    int* p = std::any_cast<int>(&a);
    assert(p && *p == 10);
    assert(std::any_cast<std::string>(&a) == nullptr);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_any", run>;

}  // namespace
