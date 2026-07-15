// Topic    : headers map · 迭代器 / 范围 / 算法
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : iterators_ranges_algorithms_headers
// Topic id : part3/section08/iterators_ranges_algorithms_headers
// Headers  : <iterator> <ranges> <algorithm> <execution> <generator>(C++23)

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <execution>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <version>

#if defined(__has_include)
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR 1
#endif
#endif
#ifndef LEARN_HAS_GENERATOR
#define LEARN_HAS_GENERATOR 0
#endif

#if !defined(__cpp_lib_generator) || !(__cpp_lib_generator)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::generator not available (__cpp_lib_generator)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/iterators_ranges_algorithms_headers", run>;
}  // namespace
#else
namespace {

#if LEARN_HAS_GENERATOR
std::generator<int> count_to(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i;
    }
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iterators_ranges_algorithms_headers] map probe ===\n";
    std::cout << "<iterator>  iterator concepts, inserters, ssize\n";
    std::cout << "<ranges>    views/adaptors/ranges::to (C++20/23)\n";
    std::cout << "<algorithm> sort/find + ranges algorithms\n";
    std::cout << "<execution> parallel policies (C++17)\n";
    std::cout << "<generator> C++23 coroutine view\n";

    std::vector<int> v{3, 1, 4, 1, 5};
    assert(std::ranges::count(v, 1) == 2);
    std::ranges::sort(v);
    assert(v.front() == 1 && v.back() == 5);

    auto even = v | std::views::filter([](int x) { return x % 2 == 0; });
    std::vector<int> evens;
    std::ranges::copy(even, std::back_inserter(evens));
    assert(!evens.empty());
    std::cout << "even count=" << evens.size() << " first=" << evens.front() << '\n';

    assert(std::ssize(v) == 5);
    [[maybe_unused]] auto it = std::ranges::find(v, 4);
    assert(it != v.end());

    // execution 策略符号存在性（未必真并行）
    std::vector<int> w = v;
    std::sort(std::execution::seq, w.begin(), w.end());
    assert(std::ranges::is_sorted(w));
    std::cout << "execution::seq sort ok\n";

#if LEARN_HAS_GENERATOR
    int sum = 0;
    for (int x : count_to(3)) {
        sum += x;
    }
    assert(sum == 6);
    std::cout << "generator sum=" << sum << '\n';
#else
    std::cout << "<generator> not available\n";
#endif

    std::cout << "[iterators_ranges_algorithms_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/iterators_ranges_algorithms_headers", run>;

}  // namespace
#endif  // __cpp_lib_generator
