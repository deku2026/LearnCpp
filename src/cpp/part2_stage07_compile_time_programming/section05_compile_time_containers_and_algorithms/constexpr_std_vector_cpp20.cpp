// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_std_vector_cpp20
// Topic id : part2/stage07/section05/constexpr_std_vector_cpp20
//
// Covers: constexpr std::vector (C++20) with feature test

#include "learn/topic_registry.hpp"

#include <cassert>
#include <vector>
#include <version>

namespace {

#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L
constexpr int sum_vector() {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    int s = 0;
    for (int x : v) {
        s += x;
    }
    return s;
}
#endif

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    assert(v.size() == 3);
    assert(v[0] + v[1] + v[2] == 6);
}

void demo_intermediate() {
#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L
    constexpr int s = sum_vector();
    static_assert(s == 6);
    assert(s == 6);
#else
    std::vector<int> v{1, 2, 3};
    int s = 0;
    for (int x : v) {
        s += x;
    }
    assert(s == 6);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L
    constexpr auto n = []() constexpr {
        std::vector<int> v(4, 10);
        v.resize(2);
        return static_cast<int>(v.size()) + v[0];
    }();
    static_assert(n == 12);
    assert(n == 12);
#else
    std::vector<int> v(4, 10);
    v.resize(2);
    assert(static_cast<int>(v.size()) + v[0] == 12);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_std_vector_cpp20", run>;

}  // namespace
