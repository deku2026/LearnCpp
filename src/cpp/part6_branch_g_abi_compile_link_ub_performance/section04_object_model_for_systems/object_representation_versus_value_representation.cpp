// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : object_representation_versus_value_representation
// Topic id : part6/g/section04/object_representation_versus_value_representation
//
// Covers: object vs value representation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <type_traits>

namespace {

struct P {
    int x;
    int y;
};

void demo_basics() {
    static_assert(std::is_trivially_copyable_v<P>);
    P a{1, 2};
    P b{};
    std::memcpy(&b, &a, sizeof(P));
    assert(b.x == 1 && b.y == 2);
}

void demo_intermediate() {
    assert(sizeof(P) == sizeof(int) * 2);
}

void demo_expert() {
    // Value representation may ignore padding; comparisons on padding are meaningless.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/g/section04/object_representation_versus_value_representation", run>;

}  // namespace
