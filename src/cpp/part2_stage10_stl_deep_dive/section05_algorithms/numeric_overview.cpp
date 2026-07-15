// Topic     : <numeric> 全景：accumulate / reduce / iota / partial_sum / inner_product
// Doc       : 第2部分-阶段10 · 步骤 5.1
// cppreference: https://en.cppreference.com/cpp/numeric
//
// 要点: accumulate 顺序折叠; reduce 可不保证顺序; iota 填递增序列。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [numeric_overview] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

    // accumulate: 左折叠, 保证顺序
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    const int prod = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{});
    assert(sum == 15);
    assert(prod == 120);

    // 字符串拼接折叠
    std::vector<std::string> words{"C++", " ", "23"};
    const auto phrase = std::accumulate(words.begin(), words.end(), std::string{});
    assert(phrase == "C++ 23");
    std::cout << "[accumulate] sum=" << sum << " prod=" << prod << " phrase=\"" << phrase << "\"\n";

    // iota: 填递增
    std::vector<int> seq(5);
    std::iota(seq.begin(), seq.end(), 10);
    assert((seq == std::vector<int>{10, 11, 12, 13, 14}));

    // partial_sum: 前缀和
    std::vector<int> prefix(v.size());
    std::partial_sum(v.begin(), v.end(), prefix.begin());
    assert((prefix == std::vector<int>{1, 3, 6, 10, 15}));

    // adjacent_difference: 相邻差
    std::vector<int> diffs(v.size());
    std::adjacent_difference(v.begin(), v.end(), diffs.begin());
    // 首元素原样拷贝, 其后为差值
    assert((diffs == std::vector<int>{1, 1, 1, 1, 1}));

    // inner_product: 点积
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    [[maybe_unused]] const int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    assert(dot == 1 * 4 + 2 * 5 + 3 * 6);  // 32

    // transform_reduce (C++17): 先 map 再 reduce
    [[maybe_unused]] const int sum_sq =
        std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [](int x) { return x * x; });
    assert(sum_sq == 1 + 4 + 9 + 16 + 25);

    // inclusive/exclusive scan (C++17)
    std::vector<int> incl(v.size()), excl(v.size());
    std::inclusive_scan(v.begin(), v.end(), incl.begin());
    std::exclusive_scan(v.begin(), v.end(), excl.begin(), 0);
    assert(incl == prefix);
    assert((excl == std::vector<int>{0, 1, 3, 6, 10}));

    std::cout << "[numeric] iota/partial_sum/inner_product/transform_reduce OK\n";
    std::cout << "numeric_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/numeric_overview", run>;

}  // namespace
