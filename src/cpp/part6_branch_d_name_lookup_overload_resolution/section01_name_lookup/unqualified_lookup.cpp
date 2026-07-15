// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : unqualified_lookup
// Topic id : part6/d/section01/unqualified_lookup
//
// Covers: unqualified name lookup ordinary rules

#include "learn/topic_registry.hpp"

namespace {

namespace outer {
int value = 1;
namespace inner {
int value = 2;
int read() {
    return value;
}  // finds inner::value
}  // namespace inner
}  // namespace outer

int value = 0;

void demo_basics() {
    LEARN_CHECK(value == 0);
    LEARN_CHECK(outer::value == 1);
    LEARN_CHECK(outer::inner::value == 2);
}

void demo_intermediate() {
    LEARN_CHECK(outer::inner::read() == 2);
    int value = 9;
    LEARN_CHECK(value == 9);  // local hides global
}

void demo_expert() {
    {
        using outer::value;
        LEARN_CHECK(value == 1);
    }
    LEARN_CHECK(::value == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/unqualified_lookup", run>;

}  // namespace
