// Topic    : 插入迭代器 back/front/inserter + erase 惯用法
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 4.3 / 5.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : insert_erase_iterators
// Topic id : part2/stage10/section04/insert_erase_iterators
// Refs     : https://en.cppreference.com/w/cpp/iterator/back_inserter
//            https://en.cppreference.com/w/cpp/iterator/front_inserter
//            https://en.cppreference.com/w/cpp/iterator/inserter
//            https://en.cppreference.com/w/cpp/container/vector/erase2

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [insert_erase_iterators] 入门：back_inserter ===\n";
    {
        std::vector<int> src{1, 2, 3};
        std::vector<int> dst;
        std::copy(src.begin(), src.end(), std::back_inserter(dst));
        assert(dst == src);
        // back_inserter：operator= 变成 push_back
        std::cout << "back_inserter turns assign into push_back\n";
    }

    std::cout << "=== 入门：front_inserter / inserter ===\n";
    {
        std::deque<int> d;
        std::vector<int> src{1, 2, 3};
        std::copy(src.begin(), src.end(), std::front_inserter(d));
        // front 插入逆序
        assert((d == std::deque<int>{3, 2, 1}));

        std::list<int> lst{10, 40};
        auto pos = std::next(lst.begin());  // 指向 40
        std::vector<int> mid{20, 30};
        std::copy(mid.begin(), mid.end(), std::inserter(lst, pos));
        assert((lst == std::list<int>{10, 20, 30, 40}));
        std::cout << "front_inserter/inserter for non-back growth\n";
    }

    std::cout << "=== 进阶：inserter 用于 set（按序插入）===\n";
    {
        std::set<int> s{1, 5};
        std::vector<int> more{3, 2, 4};
        std::copy(more.begin(), more.end(), std::inserter(s, s.end()));
        assert((s == std::set<int>{1, 2, 3, 4, 5}));
        std::cout << "inserter(s, hint) works with associative containers\n";
    }

    std::cout << "=== 进阶：erase-remove 惯用法（算法不改容器 size）===\n";
    {
        std::vector<int> v{1, 2, 3, 2, 4, 2, 5};
        // remove 把“保留元素”搬到前面，返回新逻辑 end
        auto logical_end = std::remove(v.begin(), v.end(), 2);
        // 此时 size 未变，尾部是搬移后的残渣
        assert(v.size() == 7);
        v.erase(logical_end, v.end());
        assert((v == std::vector<int>{1, 3, 4, 5}));
        std::cout << "remove + erase; algorithms cannot shrink containers\n";
    }

    std::cout << "=== 专家：C++20 std::erase / erase_if 一步到位 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        [[maybe_unused]] const auto n = std::erase(v, 3);  // 删所有 == 3
        assert(n == 1);
        std::erase_if(v, [](int x) { return x % 2 == 0; });
        assert((v == std::vector<int>{1, 5}));
        std::cout << "std::erase / erase_if replace erase-remove for sequence\n";
    }

    std::cout << "=== 专家：transform 到 back_inserter ===\n";
    {
        std::vector<int> in{1, 2, 3};
        std::vector<int> out;
        out.reserve(in.size());
        std::transform(in.begin(), in.end(), std::back_inserter(out), [](int x) { return x * x; });
        assert((out == std::vector<int>{1, 4, 9}));
        // 若已知大小，直接 out.resize + transform 到 out.begin() 更少分配
        std::cout << "inserter adapters glue algorithms to growing containers\n";
    }

    std::cout << "[insert_erase_iterators] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section04/insert_erase_iterators", run>;

}  // namespace
