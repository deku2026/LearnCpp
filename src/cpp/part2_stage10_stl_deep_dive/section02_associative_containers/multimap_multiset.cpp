// Topic    : multimap/multiset：允许重复键的有序关联容器
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : multimap_multiset
// Topic id : part2/stage10/section02/multimap_multiset
// Refs     : https://en.cppreference.com/w/cpp/container/multimap
//            https://en.cppreference.com/w/cpp/container/multiset
//            ISO [multimap] [multiset]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [multimap_multiset] 入门：multiset 允许重复 ===\n";
    {
        std::multiset<int> ms{1, 2, 2, 2, 3};
        assert(ms.size() == 5);
        assert(ms.count(2) == 3);
        assert(ms.contains(2));
        // 相等元素相邻（等价区间）
        auto [lo, hi] = ms.equal_range(2);
        assert(std::distance(lo, hi) == 3);
        std::cout << "multiset: duplicates kept, grouped by equivalence\n";
    }

    std::cout << "=== 入门：multimap 同一 key 多 value ===\n";
    {
        std::multimap<std::string, int> mm;
        mm.emplace("alice", 90);
        mm.emplace("bob", 80);
        mm.emplace("alice", 95);  // 同一 key 再插一条
        assert(mm.count("alice") == 2);
        assert(mm.size() == 3);

        // ⚠️ multimap 没有 operator[] / at（key 不唯一）
        int sum = 0;
        auto [lo, hi] = mm.equal_range("alice");
        for (auto it = lo; it != hi; ++it) {
            sum += it->second;
        }
        assert(sum == 185);
        std::cout << "equal_range walks all values for one key\n";
    }

    std::cout << "=== 进阶：erase(key) 删全部等价键；erase(it) 删一条 ===\n";
    {
        std::multiset<int> ms{1, 2, 2, 2, 3};
        auto it = ms.find(2);  // 任一 2
        ms.erase(it);          // 只删一个
        assert(ms.count(2) == 2);

        ms.erase(2);  // 删掉所有 2
        assert(ms.count(2) == 0);
        assert((ms == std::multiset<int>{1, 3}));
        std::cout << "erase(iterator) vs erase(key) cardinality differs\n";
    }

    std::cout << "=== 进阶：与 map 对照 — insert 永不“失败于重复”===\n";
    {
        std::map<int, char> m;
        std::multimap<int, char> mm;
        auto [i1, ok1] = m.insert({1, 'a'});
        auto [i2, ok2] = m.insert({1, 'b'});
        assert(ok1 && !ok2 && i2->second == 'a');

        mm.insert({1, 'a'});
        mm.insert({1, 'b'});
        assert(mm.count(1) == 2);
        std::cout << "map insert may fail; multimap always inserts\n";
    }

    std::cout << "=== 专家：稳定等价序与 lower/upper 区间算法 ===\n";
    {
        // 记录“事件时间戳”允许多个相同时间
        std::multimap<int, std::string> events{
            {10, "start"},
            {20, "tick"},
            {20, "tick2"},
            {30, "end"},
        };
        // 查询 [15, 25) 时间窗
        auto lo = events.lower_bound(15);
        auto hi = events.lower_bound(25);
        int n = 0;
        for (auto it = lo; it != hi; ++it) {
            ++n;
            assert(it->first >= 15 && it->first < 25);
        }
        assert(n == 2);
        std::cout << "ordered multi* shines for range queries with dup keys\n";
    }

    std::cout << "=== 专家：extract 单节点 / merge ===\n";
    {
        std::multiset<int> a{1, 1, 2};
        std::multiset<int> b{2, 3};
        a.merge(b);  // 节点搬迁
        assert(b.empty());
        assert(a.count(1) == 2 && a.count(2) == 2 && a.count(3) == 1);

        auto nh = a.extract(a.find(3));
        assert(nh.value() == 3);
        assert(!a.contains(3));
        std::cout << "merge/extract move nodes without copy of elements\n";
    }

    std::cout << "[multimap_multiset] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02/multimap_multiset", run>;

}  // namespace
