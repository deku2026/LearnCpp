// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.3 ranges::fold_* (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : ranges_fold_overview_cpp23
// Topic id : part3/section02/ranges_fold_overview_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/algorithm/ranges/fold_left
//   Feature-test: __cpp_lib_ranges_fold >= 202207L
//
// 要点: fold_left / fold_right 归约; 比 accumulate 更 ranges 原生。

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<algorithm>)
#include <algorithm>
#endif
#if __has_include(<ranges>)
#include <ranges>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/ranges_fold_overview_cpp23 ===\n";

    const std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    const int sum = std::ranges::fold_left(v, 0, std::plus<>{});
    assert(sum == 15);

    [[maybe_unused]] const int prod = std::ranges::fold_left(v, 1, std::multiplies<>{});
    assert(prod == 120);

    // fold_left_first: 用首元素作初值(空 range 返回 nullopt)
    [[maybe_unused]] const auto sum2 = std::ranges::fold_left_first(v, std::plus<>{});
    assert(sum2.has_value() && *sum2 == 15);

    // 字符串拼接
    const std::vector<std::string> words{"a", "b", "c"};
    const auto joined = std::ranges::fold_left(words, std::string{}, [](std::string acc, const std::string& w) {
        if (!acc.empty()) {
            acc += '-';
        }
        acc += w;
        return acc;
    });
    assert(joined == "a-b-c");

    std::cout << "[intro] fold_left sum=" << sum << " joined=" << joined << '\n';

    // fold_right: 从右结合
    const int r = std::ranges::fold_right(v, 0, std::plus<>{});
    assert(r == 15);
    std::cout << "[advanced] fold_right also " << r << '\n';
#else
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    assert(sum == 15);
    std::cout << "[intro] ranges_fold unavailable; accumulate sum=" << sum << '\n';
#endif

    std::cout << "[expert] fold is the ranges-native reduce; pick left/right "
                 "by associativity needs\n";
    std::cout << "ranges_fold_overview_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/ranges_fold_overview_cpp23", run>;

}  // namespace
