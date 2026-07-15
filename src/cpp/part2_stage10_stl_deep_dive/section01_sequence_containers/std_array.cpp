// Topic    : std::array：编译期固定大小的连续容器
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.3
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_array
// Topic id : part2/stage10/section01/std_array
// Refs     : https://en.cppreference.com/w/cpp/container/array
//            https://en.cppreference.com/w/cpp/container/array/to_array
//            ISO [array]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_array] 入门：固定大小 + 值语义 ===\n";
    {
        std::array<int, 4> a{1, 2, 3, 4};
        assert(a.size() == 4);
        assert(a[0] == 1);
        assert(a.front() == 1 && a.back() == 4);
        assert(!a.empty());  // N>0 时 empty 恒 false

        // 可拷贝（C 数组不能直接赋值）
        auto b = a;
        b[0] = 99;
        assert(a[0] == 1);
        assert(b[0] == 99);
        std::cout << "array is copyable aggregate, size is constexpr\n";
    }

    std::cout << "=== 入门：与 C 数组互操作 data()/get ===\n";
    {
        std::array<int, 3> a{10, 20, 30};
        int* p = a.data();
        assert(p[1] == 20);

        assert(std::get<0>(a) == 10);
        assert(std::get<2>(a) == 30);
        static_assert(std::tuple_size_v<decltype(a)> == 3);
        static_assert(std::is_same_v<std::tuple_element_t<1, decltype(a)>, int>);
        std::cout << "tuple protocol: get/tuple_size/tuple_element\n";
    }

    std::cout << "=== 进阶：算法可用（随机访问 / 连续）===\n";
    {
        std::array<int, 5> a{5, 1, 4, 2, 3};
        std::sort(a.begin(), a.end());
        assert((a == std::array<int, 5>{1, 2, 3, 4, 5}));

        const int sum = std::accumulate(a.begin(), a.end(), 0);
        assert(sum == 15);

        // 结构化绑定
        std::array<int, 3> coords{1, 2, 3};
        auto [x, y, z] = coords;
        assert(x == 1 && y == 2 && z == 3);
        std::cout << "sort/accumulate/structured binding work\n";
    }

    std::cout << "=== 进阶：at 边界检查 vs operator[] ===\n";
    {
        std::array<int, 2> a{7, 8};
        assert(a.at(1) == 8);
        try {
            (void)a.at(2);
            assert(false);
        } catch (const std::out_of_range&) {
            std::cout << "at(2) throws out_of_range\n";
        }
        // a[2] 是 UB，不要演示解引用
    }

    std::cout << "=== 专家：to_array / 零开销相对 C 数组 ===\n";
    {
        auto a = std::to_array({3, 1, 4});
        static_assert(a.size() == 3);
        assert(a[2] == 4);

        // size 是编译期常量，可作非类型模板参数场景
        constexpr std::array<int, 3> c{1, 2, 3};
        static_assert(c[1] == 2);
        static_assert(std::size(c) == 3);

        // fill
        std::array<int, 4> z{};
        z.fill(7);
        assert(z[0] == 7 && z[3] == 7);
        std::cout << "to_array + constexpr array + fill\n";
    }

    std::cout << "=== 专家：何时用 array 而非 vector ===\n";
    // N 编译期已知、栈上小缓冲、嵌入式/热点路径避免堆分配
    // 需要运行时变长 → vector
    std::cout << "fixed N + no heap → array; dynamic size → vector\n";

    std::cout << "[std_array] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/std_array", run>;

}  // namespace
