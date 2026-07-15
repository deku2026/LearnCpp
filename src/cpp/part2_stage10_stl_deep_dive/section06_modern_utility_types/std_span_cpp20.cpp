// Topic     : std::span —— 连续序列的不拥有可写视图 (C++20)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 7.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section06_modern_utility_types
// Item      : std_span_cpp20
// Topic id  : part2/stage10/section06/std_span_cpp20
// Refs      : https://en.cppreference.com/w/cpp/container/span
//             P0122

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>
#include <ranges>
#include <span>
#include <vector>

namespace {

void scale(std::span<int> s, int factor) {
    for (int& x : s) x *= factor;
}

[[maybe_unused]] int sum_const(std::span<const int> s) {
    int total = 0;
    for (int x : s) total += x;
    return total;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_span_cpp20] 入门：统一 vector/array/C 数组参数 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        std::array<int, 3> a{10, 20, 30};
        int carr[] = {100, 200};

        scale(v, 2);
        scale(a, 2);
        scale(carr, 2);
        assert((v == std::vector<int>{2, 4, 6, 8, 10}));
        assert(a[0] == 20 && a[2] == 60);
        assert(carr[0] == 200 && carr[1] == 400);

        assert(sum_const(v) == 30);
        assert(sum_const(a) == 120);
        std::cout << "unified scale/sum OK\n";
    }

    std::cout << "=== 进阶：subspan / first / last + 静/动态 extent ===\n";
    {
        std::vector<int> v{2, 4, 6, 8, 10};
        std::span<int> sp = v;
        [[maybe_unused]] auto sub = sp.subspan(1, 3);  // [4,6,8]
        assert(sub.size() == 3 && sub[0] == 4 && sub[2] == 8);
        assert(sp.first(2)[1] == 4);
        assert(sp.last(2)[0] == 8);

        std::array<int, 3> a{1, 2, 3};
        std::span<int, 3> fixed = a;
        static_assert(fixed.extent == 3);
        assert(fixed.size() == 3);

        std::span<int> dyn = v;
        static_assert(dyn.extent == std::dynamic_extent);
        assert(dyn.size() == 5);
        std::cout << "slicing + extent OK\n";
    }

    std::cout << "=== 专家：悬垂陷阱 + borrowed_range + 替代 T*/size ===\n";
    {
        // ⚠️ 勿绑定临时 initializer_list
        // std::span<const int> bad = {1, 2, 3}; // 临时销毁 → 悬垂

        std::vector<int> owned{1, 2, 3};
        std::span<int> live = owned;
        assert(live.data() == owned.data());

        // span 是 borrowed_range（迭代器可比 span 对象活得久——若底层仍在）
        static_assert(std::ranges::borrowed_range<std::span<int>>);

        // 函数参数最佳实践：只在调用栈内用 span，不存为成员指向临时
        [[maybe_unused]] auto as_bytes_size = [](std::span<const int> s) { return s.size_bytes(); };
        assert(as_bytes_size(owned) == 3 * sizeof(int));

        // 空 span
        [[maybe_unused]] std::span<int> empty{};
        assert(empty.empty() && empty.size() == 0);
        std::cout << "dangling discipline + size_bytes OK\n";
    }

    std::cout << "[std_span_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_span_cpp20", run>;

}  // namespace
