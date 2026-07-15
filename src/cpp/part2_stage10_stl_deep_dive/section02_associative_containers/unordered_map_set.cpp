// Topic    : unordered_map/set 哈希表（平均 O(1)，无序）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : unordered_map_set
// Topic id : part2/stage10/section02/unordered_map_set
// Refs     : https://en.cppreference.com/w/cpp/container/unordered_map
//            https://en.cppreference.com/w/cpp/container/unordered_set
//            ISO [unord.map] [unord.set]

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace {

struct Point {
    int x = 0;
    int y = 0;
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
};

struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        // 简单组合哈希（教学用；生产可用更好混合）
        return std::hash<int>{}(p.x) * 1315423911u + std::hash<int>{}(p.y);
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [unordered_map_set] 入门：平均 O(1) 查找，无序 ===\n";
    {
        std::unordered_map<std::string, int> um{
            {"apple", 1},
            {"banana", 2},
        };
        assert(um["apple"] == 1);
        um["cherry"] = 3;
        assert(um.at("banana") == 2);
        assert(um.contains("cherry"));

        std::unordered_set<int> us{3, 1, 4, 1, 5};
        assert(us.size() == 4);
        assert(us.contains(4));
        std::cout << "hash containers: fast lookup, iteration order unspecified\n";
    }

    std::cout << "=== 进阶：桶、负载因子、rehash/reserve ===\n";
    {
        std::unordered_map<int, int> um;
        um.max_load_factor(1.0f);
        [[maybe_unused]] const auto buckets0 = um.bucket_count();
        um.reserve(100);  // 预留至少装 100 元素而不超 max_load_factor
        assert(um.bucket_count() >= buckets0);
        assert(um.bucket_count() >= 100);  // 通常成立

        for (int i = 0; i < 50; ++i) {
            um.emplace(i, i * i);
        }
        assert(um.size() == 50);
        assert(um.load_factor() <= um.max_load_factor() + 1e-6f);
        std::cout << "size=" << um.size() << " buckets=" << um.bucket_count() << " load_factor=" << um.load_factor()
                  << '\n';

        [[maybe_unused]] const auto* p = &um.at(0);
        um.rehash(um.bucket_count() * 2);  // 强制重建桶
        // rehash：迭代器全失效，但引用/指针对元素仍有效（节点式哈希）
        assert(*p == 0);
        std::cout << "rehash invalidates iterators, keeps references\n";
    }

    std::cout << "=== 进阶：自定义 key 的 hash + equal ===\n";
    {
        std::unordered_set<Point, PointHash> pts;
        pts.insert(Point{1, 2});
        pts.insert(Point{1, 2});  // 重复
        pts.insert(Point{3, 4});
        assert(pts.size() == 2);
        assert(pts.contains(Point{1, 2}));
        std::cout << "user key needs Hash + EqualityComparable\n";
    }

    std::cout << "=== 专家：有序 vs 无序选择 ===\n";
    {
        // 需要有序遍历 / 范围查询 → map/set
        // 只要点查、不关心顺序 → unordered_* 平均更快
        // 最坏 O(n)（恶意哈希/极差 hash）；C++ 标准未强制防碰撞
        std::unordered_map<int, int> um;
        for (int i = 0; i < 10; ++i) {
            um[i] = i;
        }
        // 遍历顺序 ≠ 插入顺序（实现相关）
        [[maybe_unused]] int sum = 0;
        for (const auto& [k, v] : um) {
            sum += k + v;
            (void)v;
        }
        assert(sum == 2 * (0 + 9) * 10 / 2);
        std::cout << "prefer unordered for pure lookup; map for order/ranges\n";
    }

    std::cout << "=== 专家：node extract / merge（C++17）===\n";
    {
        std::unordered_map<std::string, int> a{{"x", 1}, {"y", 2}};
        std::unordered_map<std::string, int> b{{"y", 20}, {"z", 3}};
        a.merge(b);  // y 冲突：b 保留自己的 y
        assert(a.at("y") == 2);
        assert(b.contains("y") && b.at("y") == 20);
        assert(a.contains("z"));
        assert(!b.contains("z"));
        std::cout << "merge moves unique keys; duplicates stay in source\n";
    }

    std::cout << "[unordered_map_set] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02/unordered_map_set", run>;

}  // namespace
