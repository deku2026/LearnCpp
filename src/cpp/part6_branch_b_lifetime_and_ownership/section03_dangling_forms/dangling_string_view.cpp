// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_string_view
// Topic id : part6/b/section03/dangling_string_view
//
// Covers: string_view non-owning; bind only to living character sequences

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    std::string s = "hello";
    std::string_view sv = s;
    LEARN_CHECK(sv == "hello");
}

void demo_intermediate() {
    // Temporary string extended by const ref, then view it.
    const std::string& ext = std::string("abc");
    std::string_view sv = ext;
    LEARN_CHECK(sv.size() == 3);
}

void demo_expert() {
    // Do not: std::string_view sv = std::string("tmp"); // dangling after full-expression
    std::string owner = "tmp";
    std::string_view sv = owner;
    LEARN_CHECK(sv == "tmp");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_string_view", run>;

}  // namespace
