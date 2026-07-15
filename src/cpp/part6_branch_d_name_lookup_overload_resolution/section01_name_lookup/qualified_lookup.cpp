// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : qualified_lookup
// Topic id : part6/d/section01/qualified_lookup
//
// Covers: qualified lookup with :: and nested names

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

namespace lib {
struct Tool {
    static int id() { return 7; }
    int n = 3;
};
namespace detail {
int x = 5;
}
}  // namespace lib

void demo_basics() {
    assert(lib::detail::x == 5);
    assert(lib::Tool::id() == 7);
}

void demo_intermediate() {
    lib::Tool t;
    assert(t.n == 3);
    assert((&lib::Tool::n) != nullptr);
}

void demo_expert() {
    // Qualified lookup does not do ADL.
    assert(::lib::Tool::id() == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/qualified_lookup", run>;

}  // namespace
