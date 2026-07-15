// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : compile_time_sort
// Topic id : part2/stage07/section05/compile_time_sort
//
// Covers: constexpr bubble sort on std::array

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <utility>

namespace {

template <typename T, std::size_t N>
constexpr std::array<T, N> sorted_copy(std::array<T, N> a) {
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j + 1 < N - i; ++j) {
            if (a[j] > a[j + 1]) {
                auto tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
    return a;
}

void demo_basics() {
    constexpr auto s = sorted_copy(std::array{3, 1, 2});
    static_assert(s[0] == 1 && s[1] == 2 && s[2] == 3);
    assert(s[0] == 1);
}

void demo_intermediate() {
    constexpr auto s = sorted_copy(std::array{9, 7, 5, 3, 1});
    static_assert(s[0] == 1);
    static_assert(s[4] == 9);
    assert(s[2] == 5);
}

void demo_expert() {
    auto runtime = sorted_copy(std::array{4, 4, 2, 8});
    assert(runtime[0] == 2);
    assert(runtime[1] == 4);
    assert(runtime[2] == 4);
    assert(runtime[3] == 8);

    constexpr auto already = sorted_copy(std::array{1, 2, 3});
    static_assert(already[1] == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/compile_time_sort", run>;

}  // namespace
