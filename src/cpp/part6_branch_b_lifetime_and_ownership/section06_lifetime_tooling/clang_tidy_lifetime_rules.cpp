// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : clang_tidy_lifetime_rules
// Topic id : part6/b/section06/clang_tidy_lifetime_rules
//
// Covers: clang-tidy lifetime checks concept; write tidy-friendly code

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

std::string own(std::string s) {
    return s;
}

void demo_basics() {
    std::string s = own("ok");
    LEARN_CHECK(s == "ok");
}

void demo_intermediate() {
    // Prefer returning string by value over string_view into locals.
    std::string s = "data";
    std::string_view sv = s;
    LEARN_CHECK(sv == "data");
}

void demo_expert() {
    // clang-tidy bugprone-dangling-handle / cppcoreguidelines can flag risky views.
    const std::string& ext = std::string("x");
    LEARN_CHECK(ext == "x");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/clang_tidy_lifetime_rules", run>;

}  // namespace
