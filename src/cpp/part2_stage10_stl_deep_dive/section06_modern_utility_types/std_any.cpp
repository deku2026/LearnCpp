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
#include <string>

namespace {

void demo_basics() {
    std::any a = 42;
    LEARN_CHECK(a.has_value());
    LEARN_CHECK(std::any_cast<int>(a) == 42);
    a = std::string{"hi"};
    LEARN_CHECK(std::any_cast<std::string>(a) == "hi");
}

void demo_intermediate() {
    std::any a;
    LEARN_CHECK(!a.has_value());
    a = 3.14;
    LEARN_CHECK(a.type() == typeid(double));
    a.reset();
    LEARN_CHECK(!a.has_value());
}

void demo_expert() {
    std::any a = 10;
    int* p = std::any_cast<int>(&a);
    LEARN_CHECK(p && *p == 10);
    LEARN_CHECK(std::any_cast<std::string>(&a) == nullptr);
}

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
