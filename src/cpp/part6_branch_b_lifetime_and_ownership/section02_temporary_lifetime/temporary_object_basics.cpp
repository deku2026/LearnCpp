// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : temporary_object_basics
// Topic id : part6/b/section02/temporary_object_basics
//
// Covers: temporaries destroyed at end of full-expression unless extended

#include "learn/topic_registry.hpp"

#include <string>

namespace {

int make() {
    return 42;
}

void demo_basics() {
    LEARN_CHECK(make() + 1 == 43);
    std::string s = std::string("he") + "llo";
    LEARN_CHECK(s == "hello");
}

void demo_intermediate() {
    const std::string& ext = std::string("temp");
    LEARN_CHECK(ext == "temp");  // lifetime extended to scope of ext
}

void demo_expert() {
    // Full-expression end destroys unextended temporaries.
    int sum = 0;
    sum += std::string("ab").size();
    LEARN_CHECK(sum == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/temporary_object_basics", run>;

}  // namespace
