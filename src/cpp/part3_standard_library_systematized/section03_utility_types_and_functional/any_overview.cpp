// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : any_overview
// Topic id : part3/section03/any_overview
//
// Covers: std::any type-erased value

#include "learn/topic_registry.hpp"

#include <any>
#include <cassert>
#include <string>
#include <typeinfo>

namespace {

void demo_basics() {
    std::any a = 42;
    assert(std::any_cast<int>(a) == 42);
    a = std::string{"x"};
    assert(std::any_cast<std::string>(a) == "x");
}

void demo_intermediate() {
    std::any a;
    assert(!a.has_value());
    a = 1.5;
    assert(a.type() == typeid(double));
}

void demo_expert() {
    std::any a = 7;
    try {
        (void)std::any_cast<std::string>(a);
        assert(false);
    } catch (const std::bad_any_cast&) {
        assert(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/any_overview", run>;

}  // namespace
