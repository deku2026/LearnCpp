// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : typename_disambiguation
// Topic id : part6/d/section02/typename_disambiguation
//
// Covers: typename required for dependent nested types

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <vector>

namespace {

template <class C>
typename C::value_type first_or_default(const C& c) {
    if (c.empty()) return typename C::value_type{};
    return *c.begin();
}

void demo_basics() {
    std::vector<int> v{10, 20};
    LEARN_CHECK(first_or_default(v) == 10);
}

void demo_intermediate() {
    std::vector<int> empty;
    LEARN_CHECK(first_or_default(empty) == 0);
}

void demo_expert() {
    static_assert(std::is_same_v<typename std::vector<int>::value_type, int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section02/typename_disambiguation", run>;

}  // namespace
