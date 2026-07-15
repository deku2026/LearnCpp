// Topic     : std::span —— 连续序列的不拥有可写视图 (C++20)
// Doc       : 第2部分-阶段10 · 步骤 7.2
// cppreference: https://en.cppreference.com/cpp/container/span
//
// 要点: 泛化 string_view; 可写; 静态/动态 extent; 函数参数统一接口。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <span>
#include <vector>

namespace {

void scale(std::span<int> s, int factor) {
    for (int& x : s) x *= factor;
}

int sum_const(std::span<const int> s) {
    int total = 0;
    for (int x : s) total += x;
    return total;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_span_cpp20] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};
    std::array<int, 3> a{10, 20, 30};
    int carr[] = {100, 200};

    scale(v, 2);
    scale(a, 2);
    scale(carr, 2);
    assert((v == std::vector<int>{2, 4, 6, 8, 10}));
    assert(a[0] == 20 && carr[1] == 400);

    assert(sum_const(v) == 30);
    assert(sum_const(a) == 20 + 40 + 60);

    // 子视图
    std::span<int> sp = v;
    auto sub = sp.subspan(1, 3);  // [4,6,8]
    assert(sub.size() == 3 && sub[0] == 4 && sub[2] == 8);
    assert(sp.first(2)[1] == 4);
    assert(sp.last(2)[0] == 8);

    // 静态 extent: 大小进类型
    std::span<int, 3> fixed = a;
    static_assert(fixed.extent == 3);
    assert(fixed.size() == 3);

    // 动态 extent
    std::span<int> dyn = v;
    static_assert(dyn.extent == std::dynamic_extent);
    assert(dyn.size() == 5);

    // ⚠️ 悬垂: 勿绑定临时 initializer_list
    // std::span<const int> bad = {1, 2, 3}; // 临时销毁 → 悬垂

    std::cout << "[span] vector/array/C-array + subspan/first/last OK\n";
    std::cout << "std_span_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_span_cpp20", run>;

}  // namespace
