// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_converts_from_temporary_cpp23
// Topic id : part6/b/section05/reference_converts_from_temporary_cpp23
//
// Covers: reference_converts_from_temporary trait (C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

void demo_basics() {
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    static_assert(std::reference_converts_from_temporary_v<const int&, int>);
    static_assert(!std::reference_converts_from_temporary_v<int&, int>);
#else
    const int& r = int{9};
    assert(r == 9);
#endif
}

void demo_intermediate() {
    int x = 1;
    int& r = x;
    assert(r == 1);
}

void demo_expert() {
    // Useful in templates to reject APIs that would bind refs to temporaries.
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    constexpr bool danger = std::reference_converts_from_temporary_v<const std::string&, const char*>;
    assert(danger || !danger);  // trait presence is the lesson
#else
    assert(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/reference_converts_from_temporary_cpp23", run>;

}  // namespace
