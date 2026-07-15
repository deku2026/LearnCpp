// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_constructs_from_temporary_cpp23
// Topic id : part6/b/section05/reference_constructs_from_temporary_cpp23
//
// Covers: reference_constructs_from_temporary trait (C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

void demo_basics() {
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    static_assert(std::reference_constructs_from_temporary_v<const int&, int>);
    static_assert(!std::reference_constructs_from_temporary_v<const int&, int&>);
#else
    // Conceptual: binding const int& to prvalue int materializes a temporary.
    const int& r = 5;
    assert(r == 5);
#endif
}

void demo_intermediate() {
    int x = 3;
    const int& r = x;  // no temporary
    assert(r == 3);
}

void demo_expert() {
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    static_assert(std::reference_constructs_from_temporary_v<const double&, int>);
#else
    const double& r = 1;  // temporary double
    assert(r == 1.0);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/reference_constructs_from_temporary_cpp23", run>;

}  // namespace
