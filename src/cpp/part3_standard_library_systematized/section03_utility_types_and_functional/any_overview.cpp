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
#include <string>
#include <typeinfo>

namespace {

void demo_basics() {
    std::any a = 42;
    LEARN_CHECK(std::any_cast<int>(a) == 42);
    a = std::string{"x"};
    LEARN_CHECK(std::any_cast<std::string>(a) == "x");
}

void demo_intermediate() {
    std::any a;
    LEARN_CHECK(!a.has_value());
    a = 1.5;
    LEARN_CHECK(a.type() == typeid(double));
}

void demo_expert() {
    std::any a = 7;
    try {
        (void)std::any_cast<std::string>(a);
        LEARN_CHECK(false);
    } catch (const std::bad_any_cast&) {
        LEARN_CHECK(true);
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
