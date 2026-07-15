// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part2/stage07/section05/constexpr_unique_ptr_cpp23
//
// Covers: constexpr unique_ptr / make_unique (C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <version>

namespace {

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
constexpr int compute() {
    auto p = std::make_unique<int>(21);
    return *p * 2;
}
#endif

void demo_basics() {
    auto p = std::make_unique<int>(42);
    assert(p && *p == 42);
}

void demo_intermediate() {
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    constexpr int v = compute();
    static_assert(v == 42);
    assert(v == 42);
#else
    auto p = std::make_unique<int>(21);
    assert(*p * 2 == 42);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    constexpr auto s = []() constexpr {
        auto a = std::make_unique<int>(3);
        auto b = std::make_unique<int>(4);
        return *a + *b;
    }();
    static_assert(s == 7);
    assert(s == 7);
#else
    auto a = std::make_unique<int>(3);
    auto b = std::make_unique<int>(4);
    assert(*a + *b == 7);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_unique_ptr_cpp23", run>;

}  // namespace
