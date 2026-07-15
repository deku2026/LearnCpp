// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : function_template_in_overload
// Topic id : part6/d/section03/function_template_in_overload
//
// Covers: function templates participate in overload resolution

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

int id(int x) {
    return x;
}

template <class T>
T id(T x) {
    return x;
}

void demo_basics() {
    assert(id(3) == 3);  // non-template preferred when equal
}

void demo_intermediate() {
    assert(id(std::string{"a"}) == "a");
}

void demo_expert() {
    // Non-template wins over template if both equally good.
    assert(id(5) == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/function_template_in_overload", run>;

}  // namespace
