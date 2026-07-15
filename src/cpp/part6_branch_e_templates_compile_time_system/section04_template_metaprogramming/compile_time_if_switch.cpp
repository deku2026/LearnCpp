// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : compile_time_if_switch
// Topic id : part6/e/section04/compile_time_if_switch
//
// Covers: if constexpr and compile-time branching

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

template <class T>
auto describe(T v) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<int>(v);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return static_cast<int>(v.size());
    } else {
        return 0;
    }
}

void demo_basics() {
    assert(describe(5) == 5);
}

void demo_intermediate() {
    assert(describe(std::string{"abc"}) == 3);
}

void demo_expert() {
    assert(describe(1.0) == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/compile_time_if_switch", run>;

}  // namespace
