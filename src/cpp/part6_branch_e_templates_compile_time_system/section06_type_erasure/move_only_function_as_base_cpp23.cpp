// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : move_only_function_as_base_cpp23
// Topic id : part6/e/section06/move_only_function_as_base_cpp23
//
// Covers: std::move_only_function (C++23) vs std::function

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <utility>

namespace {

void demo_basics() {
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    std::move_only_function<int()> f = [p = std::make_unique<int>(3)] { return *p; };
    assert(f() == 3);
#else
    // Fallback: unique_ptr in a custom move-only wrapper
    auto p = std::make_unique<int>(3);
    auto f = [p = std::move(p)] { return *p; };
    assert(f() == 3);
#endif
}

void demo_intermediate() {
    // std::function requires copyable callables; move-only types need move_only_function.
    std::function<int()> f = [] { return 1; };
    assert(f() == 1);
}

void demo_expert() {
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    std::move_only_function<int(int)> f = [](int x) { return x * 2; };
    assert(f(4) == 8);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/move_only_function_as_base_cpp23", run>;

}  // namespace
