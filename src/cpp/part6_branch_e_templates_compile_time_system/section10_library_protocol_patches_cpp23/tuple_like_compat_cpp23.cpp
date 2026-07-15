// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : tuple_like_compat_cpp23
// Topic id : part6/e/section10/tuple_like_compat_cpp23
//
// Covers: tuple-like protocol / structured bindings expansions

#include "learn/topic_registry.hpp"

#include <cassert>
#include <tuple>
#include <utility>

namespace {

void demo_basics() {
    auto t = std::make_tuple(1, 2);
    auto [a, b] = t;
    assert(a == 1 && b == 2);
}

void demo_intermediate() {
    std::pair<int, int> p{3, 4};
    auto [x, y] = p;
    assert(x + y == 7);
}

void demo_expert() {
    static_assert(std::tuple_size_v<std::tuple<int, double>> == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section10/tuple_like_compat_cpp23", run>;

}  // namespace
