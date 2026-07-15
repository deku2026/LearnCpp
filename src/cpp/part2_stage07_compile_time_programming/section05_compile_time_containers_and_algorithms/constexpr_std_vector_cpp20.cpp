// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_std_vector_cpp20
// Topic id : part2/stage07/section05/constexpr_std_vector_cpp20
//
// Refs:
//   https://en.cppreference.com/w/cpp/container/vector
//   P1004R2 — Making std::vector constexpr
//   P0784R7 — More constexpr containers (transient allocation model)

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 在 constexpr 函数内使用 std::vector
// ---------------------------------------------------------------------------

constexpr int sum_vector() {
    std::vector<int> v{1, 2, 3, 4, 5};
    return std::accumulate(v.begin(), v.end(), 0);
}

constexpr int count_evens() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    return static_cast<int>(std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }));
}

// ---------------------------------------------------------------------------
// §进阶 — push_back / 动态增长 / 算法；结果必须非分配
// ---------------------------------------------------------------------------

constexpr int sieve_count_primes_below(int n) {
    // 埃氏筛：vector 当工作区
    if (n <= 2) {
        return 0;
    }
    std::vector<char> is_prime(static_cast<std::size_t>(n), 1);
    is_prime[0] = is_prime[1] = 0;
    for (int p = 2; p * p < n; ++p) {
        if (is_prime[static_cast<std::size_t>(p)]) {
            for (int k = p * p; k < n; k += p) {
                is_prime[static_cast<std::size_t>(k)] = 0;
            }
        }
    }
    return static_cast<int>(std::count(is_prime.begin(), is_prime.end(), 1));
}

constexpr auto first_three_from_vector() {
    std::vector<int> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back(i * i);
    }
    std::array<int, 3> out{v[0], v[1], v[2]};
    return out;  // array 无动态分配，可带出常量求值
}

// ---------------------------------------------------------------------------
// §专家 — transient allocation：不能泄漏到运行期
// ---------------------------------------------------------------------------
// 非法：
//   constexpr std::vector<int> g{1,2,3}; // 堆内存会逃逸出常量求值
// 合法：
//   在 constexpr 函数内分配 → 析构完毕 → 只返回标量/array/聚合。
// C++26 方向：放宽非瞬态分配（本文件不依赖）。

// constexpr std::vector<int> kIllegal{1, 2, 3};

constexpr int mutate_and_sum() {
    std::vector<int> v;
    v.reserve(8);
    v.push_back(10);
    v.emplace_back(20);
    v.insert(v.begin() + 1, 15);
    v.pop_back();  // 去掉 20
    // v == {10,15}
    int s = 0;
    for (int x : v) {
        s += x;
    }
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/constexpr_std_vector_cpp20 ===\n";

    static_assert(sum_vector() == 15);
    static_assert(count_evens() == 3);
    static_assert(sieve_count_primes_below(20) == 8);  // 2..19 共 8 个质数
    constexpr auto squares = first_three_from_vector();
    static_assert(squares[0] == 0 && squares[1] == 1 && squares[2] == 4);
    static_assert(mutate_and_sum() == 25);

    std::cout << "[intro] sum_vector=" << sum_vector() << " count_evens=" << count_evens() << '\n';
    std::cout << "[advanced] primes below 20: " << sieve_count_primes_below(20) << " first squares: " << squares[0]
              << "," << squares[1] << "," << squares[2] << '\n';
    std::cout << "[expert] mutate_and_sum=" << mutate_and_sum()
              << "; vector is transient workspace only — do not constexpr-persist it\n";

    // 运行期对照
    std::vector<int> rt{1, 2, 3, 4, 5};
    assert(std::accumulate(rt.begin(), rt.end(), 0) == 15);

    std::cout << "constexpr_std_vector_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_std_vector_cpp20", run>;

}  // namespace
