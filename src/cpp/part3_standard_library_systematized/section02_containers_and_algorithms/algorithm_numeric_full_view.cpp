// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.3 <algorithm> / <numeric>
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : algorithm_numeric_full_view
// Topic id : part3/section02/algorithm_numeric_full_view
//
// Refs:
//   https://en.cppreference.com/w/cpp/algorithm
//   https://en.cppreference.com/w/cpp/numeric
//   https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag
//
// 要点: 查找/排序/变换/划分/堆/最值; accumulate/iota/gcd; 执行策略可选。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

#if defined(__has_include)
#if __has_include(<execution>)
#include <execution>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/algorithm_numeric_full_view ===\n";

    std::vector<int> v{4, 1, 3, 2, 5};

    // 查找 / 最值 / clamp
    assert(*std::find(v.begin(), v.end(), 3) == 3);
    assert(*std::min_element(v.begin(), v.end()) == 1);
    assert(*std::max_element(v.begin(), v.end()) == 5);
    assert(std::clamp(10, 0, 5) == 5);

    // 排序族
    std::vector<int> s = v;
    std::sort(s.begin(), s.end());
    assert(std::is_sorted(s.begin(), s.end()));
    assert(std::binary_search(s.begin(), s.end(), 3));

    std::vector<int> ps = v;
    std::partial_sort(ps.begin(), ps.begin() + 2, ps.end());
    assert(ps[0] <= ps[1]);

    std::vector<int> nth = v;
    std::nth_element(nth.begin(), nth.begin() + 2, nth.end());
    // 第 3 小元素在 index 2
    assert(nth[2] == 3);

    // 变换 / 填充
    std::vector<int> doubled(v.size());
    std::transform(v.begin(), v.end(), doubled.begin(), [](int x) { return x * 2; });
    assert(doubled[0] == 8);

    // 划分
    std::vector<int> p = v;
    [[maybe_unused]] auto mid = std::partition(p.begin(), p.end(), [](int x) { return x % 2 == 0; });
    assert(std::all_of(p.begin(), mid, [](int x) { return x % 2 == 0; }));

    // 堆
    std::vector<int> h = v;
    std::make_heap(h.begin(), h.end());
    assert(h.front() == *std::max_element(h.begin(), h.end()));
    std::pop_heap(h.begin(), h.end());
    h.pop_back();

    std::cout << "[intro] algorithm catalog samples ok\n";

    // numeric
    std::vector<int> n(5);
    std::iota(n.begin(), n.end(), 1);  // 1..5
    const int sum = std::accumulate(n.begin(), n.end(), 0);
    assert(sum == 15);
    [[maybe_unused]] const int prod = std::accumulate(n.begin(), n.end(), 1, std::multiplies<int>{});
    assert(prod == 120);

    std::vector<int> partial(n.size());
    std::partial_sum(n.begin(), n.end(), partial.begin());
    assert(partial.back() == 15);

    assert(std::gcd(12, 8) == 4);
    assert(std::lcm(4, 6) == 12);

    std::cout << "[advanced] iota/accumulate/gcd/lcm ok sum=" << sum << '\n';

#if defined(__cpp_lib_execution)
    // 并行策略: 元素少时未必更快; 操作须无数据竞争
    std::vector<int> pe = v;
    std::sort(std::execution::seq, pe.begin(), pe.end());
    assert(std::is_sorted(pe.begin(), pe.end()));
    std::cout << "[expert] execution::seq sort ok (par optional on platform)\n";
#else
    std::cout << "[expert] <execution> not highlighted on this build\n";
#endif

    std::cout << "algorithm_numeric_full_view: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/algorithm_numeric_full_view", run>;

}  // namespace
