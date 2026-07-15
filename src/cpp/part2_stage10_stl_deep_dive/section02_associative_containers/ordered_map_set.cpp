// Topic    : map/set 有序关联容器（红黑树，O(log n)）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : ordered_map_set
// Topic id : part2/stage10/section02/ordered_map_set
// Refs     : https://en.cppreference.com/w/cpp/container/map
//            https://en.cppreference.com/w/cpp/container/set
//            ISO [map] [set] [associative]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ordered_map_set] 入门：set 有序去重 ===\n";
    {
        std::set<int> s{3, 1, 4, 1, 5, 9, 2, 6};
        assert(s.size() == 7);  // 1 去重
        // 中序遍历即升序
        [[maybe_unused]] int prev = 0;
        bool first = true;
        for (int x : s) {
            if (!first) {
                assert(prev < x);
            }
            first = false;
            prev = x;
        }
        assert(s.contains(4));
        assert(!s.contains(7));
        std::cout << "set: unique + sorted ascending by default less<>\n";
    }

    std::cout << "=== 入门：map 键值 + operator[] / insert / try_emplace ===\n";
    {
        std::map<std::string, int> m{{"banana", 2}, {"apple", 1}};
        // 有序：apple 在 banana 前
        assert(m.begin()->first == "apple");

        m["cherry"] = 3;  // 不存在则默认构造 value 再赋值
        m["apple"] = 10;  // 存在则覆盖
        assert(m["apple"] == 10);

        [[maybe_unused]] auto [it, ok] = m.insert({"date", 4});
        assert(ok && it->second == 4);
        [[maybe_unused]] auto [it2, ok2] = m.insert({"date", 99});  // key 已在，不覆盖
        assert(!ok2 && it2->second == 4);

        m.try_emplace("elder", 5);  // 不存在才构造
        m.try_emplace("elder", 50);
        assert(m.at("elder") == 5);
        std::cout << "map: ordered by key; [] inserts default; insert no-overwrite\n";
    }

    std::cout << "=== 进阶：查找家族 lower_bound / equal_range ===\n";
    {
        std::set<int> s{1, 3, 5, 7, 9};
        [[maybe_unused]] auto lb = s.lower_bound(4);  // 第一个 >= 4 → 5
        assert(lb != s.end() && *lb == 5);
        [[maybe_unused]] auto ub = s.upper_bound(5);  // 第一个 > 5 → 7
        assert(ub != s.end() && *ub == 7);

        std::map<int, char> m{{1, 'a'}, {2, 'b'}, {3, 'c'}};
        [[maybe_unused]] auto [lo, hi] = m.equal_range(2);
        assert(lo != m.end() && lo->first == 2);
        assert(hi != m.end() && hi->first == 3);
        std::cout << "lower/upper/equal_range enable ordered range queries\n";
    }

    std::cout << "=== 进阶：自定义比较器（降序 set）===\n";
    {
        std::set<int, std::greater<>> desc{1, 5, 3, 2};
        assert(*desc.begin() == 5);
        assert(*desc.rbegin() == 1);
        std::cout << "Compare template param changes order\n";
    }

    std::cout << "=== 专家：节点式失效 — 插删不伤其他迭代器 ===\n";
    {
        std::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
        [[maybe_unused]] auto keep = m.find(2);
        m.emplace(0, "zero");
        m.erase(1);
        assert(keep != m.end());
        assert(keep->second == "two");  // 仍有效
        // 仅被删元素的迭代器失效
        std::cout << "insert/erase invalidate only erased iterators\n";
    }

    std::cout << "=== 专家：value_type 是 pair<const Key, T> ===\n";
    {
        std::map<int, int> m{{1, 10}};
        auto it = m.begin();
        // it->first = 2;  // ❌ key 为 const，不可改（会破坏树序）
        it->second = 99;  // value 可改
        assert(m.at(1) == 99);
        // 改 key 要用 extract（C++17）再改再 insert
        auto nh = m.extract(1);
        nh.key() = 7;
        m.insert(std::move(nh));
        assert(m.size() == 1 && m.contains(7) && m.at(7) == 99);
        std::cout << "mutate key via extract/node_handle (C++17)\n";
    }

    std::cout << "[ordered_map_set] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02/ordered_map_set", run>;

}  // namespace
