// Topic    : C++17 五类迭代器标签（iterator_category）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 4.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : iterator_categories_cpp17
// Topic id : part2/stage10/section04/iterator_categories_cpp17
// Refs     : https://en.cppreference.com/w/cpp/iterator
//            https://en.cppreference.com/w/cpp/iterator/iterator_tags
//            ISO [iterator.requirements]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <class It>
using cat_t = typename std::iterator_traits<It>::iterator_category;

template <class It>
constexpr bool is_random_access_v = std::is_base_of_v<std::random_access_iterator_tag, cat_t<It>>;

template <class It>
constexpr bool is_bidirectional_v = std::is_base_of_v<std::bidirectional_iterator_tag, cat_t<It>>;

template <class It>
constexpr bool is_forward_v = std::is_base_of_v<std::forward_iterator_tag, cat_t<It>>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iterator_categories_cpp17] 入门：标签层次 ===\n";
    {
        // input ← forward ← bidirectional ← random_access
        // output 独立一支
        static_assert(std::is_base_of_v<std::input_iterator_tag, std::forward_iterator_tag>);
        static_assert(std::is_base_of_v<std::forward_iterator_tag, std::bidirectional_iterator_tag>);
        static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, std::random_access_iterator_tag>);
        std::cout << "tag hierarchy models growing capabilities\n";
    }

    std::cout << "=== 入门：常见容器的 iterator_category ===\n";
    {
        using VIt = std::vector<int>::iterator;
        using DIt = std::deque<int>::iterator;
        using AIt = std::array<int, 3>::iterator;
        using LIt = std::list<int>::iterator;
        using FIt = std::forward_list<int>::iterator;
        using MIt = std::map<int, int>::iterator;
        using SIt = std::set<int>::iterator;

        static_assert(is_random_access_v<VIt>);
        static_assert(is_random_access_v<DIt>);
        static_assert(is_random_access_v<AIt>);
        static_assert(is_bidirectional_v<LIt> && !is_random_access_v<LIt>);
        static_assert(is_forward_v<FIt> && !is_bidirectional_v<FIt>);
        static_assert(is_bidirectional_v<MIt>);
        static_assert(is_bidirectional_v<SIt>);
        std::cout << "vector/deque/array: RA; list/map/set: Bi; forward_list: Fwd\n";
    }

    std::cout << "=== 进阶：能力决定算法可用性 ===\n";
    {
        std::vector<int> v{3, 1, 2};
        std::sort(v.begin(), v.end());  // 需要 random_access
        assert((v == std::vector<int>{1, 2, 3}));

        std::list<int> lst{3, 1, 2};
        // std::sort(lst.begin(), lst.end());  // ❌ 编译失败：非随机访问
        lst.sort();  // 成员 sort
        assert((lst == std::list<int>{1, 2, 3}));

        std::reverse(lst.begin(), lst.end());  // reverse 要 bidirectional
        assert((lst == std::list<int>{3, 2, 1}));
        std::cout << "std::sort needs RA; list uses member sort; reverse needs Bi\n";
    }

    std::cout << "=== 进阶：iterator_traits 抽取 value/diff/pointer/ref ===\n";
    {
        using It = std::vector<std::string>::iterator;
        static_assert(std::is_same_v<std::iterator_traits<It>::value_type, std::string>);
        static_assert(std::is_same_v<std::iterator_traits<It>::difference_type, std::ptrdiff_t>);
        static_assert(std::is_same_v<std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>);
        std::cout << "traits is the pre-concepts customization point\n";
    }

    std::cout << "=== 专家：输入/输出迭代器（单遍）示例标签 ===\n";
    {
        using IIn = std::istream_iterator<int>;
        using IOut = std::ostream_iterator<int>;
        static_assert(std::is_same_v<cat_t<IIn>, std::input_iterator_tag> ||
                      std::is_base_of_v<std::input_iterator_tag, cat_t<IIn>>);
        static_assert(std::is_same_v<cat_t<IOut>, std::output_iterator_tag> ||
                      std::is_base_of_v<std::output_iterator_tag, cat_t<IOut>>);
        // 输入：单遍只读；输出：单遍只写（如 back_inserter）
        using Ins = decltype(std::back_inserter(std::declval<std::vector<int>&>()));
        static_assert(std::is_base_of_v<std::output_iterator_tag, cat_t<Ins>>);
        std::cout << "istream=input; ostream/back_inserter=output\n";
    }

    std::cout << "=== 专家：距离与 advance 的复杂度随类别变化 ===\n";
    {
        std::vector<int> v(100);
        [[maybe_unused]] auto a = v.begin();
        [[maybe_unused]] auto b = v.begin() + 50;
        assert(std::distance(a, b) == 50);  // RA: O(1)

        std::list<int> lst(100);
        [[maybe_unused]] auto la = lst.begin();
        auto lb = lst.begin();
        std::advance(lb, 50);  // Bi: O(n)
        assert(std::distance(la, lb) == 50);
        std::cout << "distance/advance: O(1) on RA, O(n) on weaker cats\n";
    }

    std::cout << "[iterator_categories_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section04/iterator_categories_cpp17", run>;

}  // namespace
