// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : move_only_function_cpp23
// Topic id : part2/stage03/section05/move_only_function_cpp23
//
// Covers: std::move_only_function for non-copyable callables (C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <utility>

namespace {

void demo_basics() {
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    auto p = std::make_unique<int>(42);
    std::move_only_function<int()> task = [p = std::move(p)] { return *p; };
    assert(task() == 42);
#else
    // Fallback: keep ownership outside a copyable std::function.
    auto p = std::make_unique<int>(42);
    int* raw = p.get();
    std::function<int()> task = [raw] { return *raw; };
    assert(task() == 42);
    assert(p != nullptr);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    auto p = std::make_unique<int>(7);
    std::move_only_function<int()> task = [p = std::move(p)] { return *p; };
    auto task2 = std::move(task);
    assert(task2() == 7);
    // task is empty after move — do not call (UB if empty).
    assert(static_cast<bool>(task2));
#else
    auto p = std::make_unique<int>(7);
    auto task = [v = *p] { return v; };
    auto task2 = task;
    assert(task2() == 7);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    // move_only_function accepts move-only targets; std::function does not.
    auto p = std::make_unique<int>(1);
    std::move_only_function<int()> f = [p = std::move(p)]() mutable {
        *p += 1;
        return *p;
    };
    assert(f() == 2);
    assert(f() == 3);

    // Empty call is UB — always ensure engaged before invoke.
    std::move_only_function<int()> empty;
    assert(!empty);
#else
    // Document limitation: unique_ptr capture cannot live in std::function.
    auto p = std::make_unique<int>(1);
    auto local = [v = std::move(p)]() mutable {
        *v += 1;
        return *v;
    };
    assert(local() == 2);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/move_only_function_cpp23", run>;

}  // namespace
