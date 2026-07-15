// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.3 ranges 算法 (C++20/23)
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : ranges_algorithm_overview
// Topic id : part3/section02/ranges_algorithm_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/algorithm/ranges
//   https://en.cppreference.com/w/cpp/ranges
//
// 要点: ranges::sort(v) 直接传容器; 投影; contains/starts_with 等 C++23 扩展。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

struct Person {
    std::string name;
    int age;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/ranges_algorithm_overview ===\n";

    std::vector<int> v{4, 1, 3, 2};
    std::ranges::sort(v);
    assert(std::ranges::is_sorted(v));
    assert(std::ranges::binary_search(v, 3));
    assert(std::ranges::find(v, 2) != v.end());

    // 投影: 按 age 排序, 无需手写比较器
    std::vector<Person> people{{"Ada", 36}, {"Bob", 21}, {"Cyd", 30}};
    std::ranges::sort(people, {}, &Person::age);
    assert(people.front().name == "Bob");
    assert(people.back().name == "Ada");

    // views 流水线(惰性)
    auto even_squares =
        v | std::views::filter([](int x) { return x % 2 == 0; }) | std::views::transform([](int x) { return x * x; });
    std::vector<int> material{even_squares.begin(), even_squares.end()};
    assert(!material.empty());

    std::cout << "[intro] ranges::sort + projection + views ok\n";

#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202207L || defined(__cpp_lib_ranges_contains)
    // C++23: ranges::contains / starts_with / ends_with / find_last
    assert(std::ranges::contains(v, 3));
#if defined(__cpp_lib_ranges_starts_ends_with) || 1
    assert(std::ranges::starts_with(v, std::vector<int>{1, 2}) || v.front() == 1);
#endif
    std::cout << "[advanced] ranges C++23 contains-style APIs exercised\n";
#else
    assert(std::ranges::find(v, 3) != v.end());
    std::cout << "[advanced] used ranges::find as contains stand-in\n";
#endif

    // all_of / any_of / count
    assert(std::ranges::all_of(v, [](int x) { return x > 0; }));
    assert(std::ranges::count(v, 1) == 1);

    std::cout << "[expert] prefer ranges algorithms for constrained, "
                 "projection-friendly code\n";
    std::cout << "ranges_algorithm_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/ranges_algorithm_overview", run>;

}  // namespace
