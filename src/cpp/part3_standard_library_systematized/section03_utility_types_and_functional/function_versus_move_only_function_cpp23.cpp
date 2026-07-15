// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : function_versus_move_only_function_cpp23
// Topic id : part3/section03/function_versus_move_only_function_cpp23
//
// Covers: std::function vs move_only_function C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <utility>
#include <version>

namespace {

void demo_basics() {
    std::function<int(int)> f = [](int x) { return x + 1; };
    assert(f(1) == 2);
}

void demo_intermediate() {
    std::function<int()> f;
    assert(!f);
    f = [] { return 9; };
    assert(f() == 9);
}

void demo_expert() {
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    auto up = std::make_unique<int>(5);
    std::move_only_function<int()> f = [p = std::move(up)] { return *p; };
    assert(f() == 5);
#else
    // std::function requires copyable callables; unique_ptr capture needs move-only wrapper
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/function_versus_move_only_function_cpp23", run>;

}  // namespace
