// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : concepts_versus_sfinae
// Topic id : part6/e/section03/concepts_versus_sfinae
//
// Covers: concepts vs enable_if readability

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <type_traits>

namespace {

template <class T>
    requires std::integral<T>
int modern(T x) {
    return static_cast<int>(x) + 1;
}

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
int classic(T x) {
    return static_cast<int>(x) + 1;
}

void demo_basics() {
    assert(modern(3) == 4);
    assert(classic(3) == 4);
}

void demo_intermediate() {
    assert(modern(true) == 2);
}

void demo_expert() {
    static_assert(std::integral<int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/concepts_versus_sfinae", run>;

}  // namespace
