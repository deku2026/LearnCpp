// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : qualified_lookup
// Topic id : part6/d/section01/qualified_lookup
//
// Covers: qualified lookup with :: and nested names

#include "learn/topic_registry.hpp"

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
    LEARN_CHECK(lib::detail::x == 5);
    LEARN_CHECK(lib::Tool::id() == 7);
}

void demo_intermediate() {
    lib::Tool t;
    LEARN_CHECK(t.n == 3);
    // Pointer-to-member is well-formed; apply it via .* (not a null-pointer check).
    int lib::Tool::* pm = &lib::Tool::n;
    LEARN_CHECK(t.*pm == 3);
}

void demo_expert() {
    // Qualified lookup does not do ADL.
    LEARN_CHECK(::lib::Tool::id() == 7);
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
