// Topic    : C++20 迭代器概念（含 contiguous_iterator）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 4.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : iterator_concepts_cpp20
// Topic id : part2/stage10/section04/iterator_concepts_cpp20
// Refs     : https://en.cppreference.com/w/cpp/iterator#Iterator_concepts
//            https://en.cppreference.com/w/cpp/iterator/contiguous_iterator
//            ISO [iterator.concepts]

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <concepts>
#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 用 concept 约束的教学算法：需要随机访问
template <std::random_access_iterator It>
auto midpoint_value(It first, It last) {
    assert(last - first >= 1);
    return *(first + (last - first) / 2);
}

// 需要连续内存才能安全 to_address 成指针区间
template <std::contiguous_iterator It>
const typename std::iterator_traits<It>::value_type* as_ptr(It it) {
    return std::to_address(it);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iterator_concepts_cpp20] 入门：概念代替标签检查 ===\n";
    {
        static_assert(std::input_iterator<std::vector<int>::iterator>);
        static_assert(std::forward_iterator<std::forward_list<int>::iterator>);
        static_assert(std::bidirectional_iterator<std::list<int>::iterator>);
        static_assert(std::random_access_iterator<std::vector<int>::iterator>);
        static_assert(std::random_access_iterator<std::deque<int>::iterator>);
        std::cout << "concepts refine the old five categories\n";
    }

    std::cout << "=== 入门：contiguous_iterator（C++20 新增最强）===\n";
    {
        static_assert(std::contiguous_iterator<std::vector<int>::iterator>);
        static_assert(std::contiguous_iterator<std::array<int, 4>::iterator>);
        static_assert(std::contiguous_iterator<int*>);
        static_assert(std::contiguous_iterator<std::string::iterator>);  // char 连续

        // deque 随机访问但非连续
        static_assert(std::random_access_iterator<std::deque<int>::iterator>);
        static_assert(!std::contiguous_iterator<std::deque<int>::iterator>);

        // list 双向非连续
        static_assert(!std::contiguous_iterator<std::list<int>::iterator>);
        std::cout << "vector/array/string/span: contiguous; deque: not\n";
    }

    std::cout << "=== 进阶：concept 约束函数 ===\n";
    {
        std::vector<int> v{10, 20, 30, 40, 50};
        assert(midpoint_value(v.begin(), v.end()) == 30);

        std::array<int, 3> a{1, 2, 3};
        [[maybe_unused]] const int* p = as_ptr(a.begin());
        assert(p == a.data());
        assert(*p == 1);

        // midpoint_value(list.begin(), list.end()); // ❌ 不满足 random_access
        std::cout << "constrained algorithms fail early at compile time\n";
    }

    std::cout << "=== 进阶：span / string_view 的迭代器 ===\n";
    {
        int buf[] = {1, 2, 3, 4};
        std::span<int> sp{buf};
        std::string_view sv = "abcd";
        static_assert(std::contiguous_iterator<decltype(sp.begin())>);
        static_assert(std::contiguous_iterator<decltype(sv.begin())>);
        assert(std::to_address(sp.begin()) == buf);
        std::cout << "views over contiguous ranges expose contiguous_iterator\n";
    }

    std::cout << "=== 专家：indirectly_readable / weakly_incrementable 等积木 ===\n";
    {
        // 完整迭代器概念由更小概念组合：
        // input_iterator ≈ input_or_output_iterator + indirectly_readable + ...
        static_assert(std::indirectly_readable<std::vector<int>::iterator>);
        static_assert(std::weakly_incrementable<std::vector<int>::iterator>);
        static_assert(std::incrementable<std::vector<int>::iterator>);
        static_assert(std::sentinel_for<std::vector<int>::iterator, std::vector<int>::iterator>);
        std::cout << "iterator concepts are compositional building blocks\n";
    }

    std::cout << "=== 专家：为何还要 contiguous（优化与 ABI）===\n";
    {
        // 连续 ⇒ 可交给要 T* 的 C API / memcpy / 向量化假设
        std::vector<int> v{1, 2, 3, 4};
        [[maybe_unused]] int* raw = std::to_address(v.begin());
        assert(raw[2] == 3);
        // deque 即使 random_access 也不能当一整块 T* 传出
        std::cout << "contiguous enables pointer-range interop & vectorization\n";
    }

    std::cout << "[iterator_concepts_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section04/iterator_concepts_cpp20", run>;

}  // namespace
