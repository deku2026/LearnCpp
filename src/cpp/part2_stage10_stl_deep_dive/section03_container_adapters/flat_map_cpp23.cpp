// Topic    : C++23 std::flat_map：有序平行 vector 的 map
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.2（🆕 验收点）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_map_cpp23
// Topic id : part2/stage10/section03/flat_map_cpp23
// Refs     : https://en.cppreference.com/w/cpp/container/flat_map
//            P0429R9
//            feature-test: __cpp_lib_flat_map

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <version>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#define LEARN_HAS_FLAT_MAP 1
#else
#define LEARN_HAS_FLAT_MAP 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [flat_map_cpp23] 入门：map 接口 + 连续键值 ===\n";

#if LEARN_HAS_FLAT_MAP
    {
        std::flat_map<std::string, int> fm{
            {"banana", 2},
            {"apple", 1},
        };
        assert(fm.begin()->first == "apple");  // 有序
        fm["cherry"] = 3;
        assert(fm.at("banana") == 2);
        assert(fm.contains("cherry"));
        assert(fm.size() == 3);
        std::cout << "flat_map: ordered association, vector storage\n";
    }

    std::cout << "=== 进阶：查找 O(log n) 二分；插入可能 O(n) ===\n";
    {
        std::flat_map<int, char> fm;
        fm.emplace(2, 'b');
        fm.emplace(1, 'a');
        fm.emplace(3, 'c');
        auto it = fm.find(2);
        assert(it != fm.end());
        assert(it->second == 'b');
        assert(fm[1] == 'a');
        std::cout << "binary search on sorted key vector\n";
    }

    std::cout << "=== 进阶：sorted_unique 批量构建 ===\n";
    {
        std::vector<std::pair<int, int>> kvs{{1, 10}, {2, 20}, {3, 30}};
        std::flat_map<int, int> fm{std::sorted_unique, kvs.begin(), kvs.end()};
        assert(fm.size() == 3);
        assert(fm[2] == 20);
        std::cout << "build once from sorted unique pairs\n";
    }

    std::cout << "=== 专家：extract 键/值容器（平行数组）===\n";
    {
        std::flat_map<int, std::string> fm{{1, "one"}, {2, "two"}};
        auto containers = std::move(fm).extract();
        // containers.keys / containers.values（实现为 struct of containers）
        assert(containers.keys.size() == 2);
        assert(containers.values.size() == 2);
        std::cout << "two parallel vectors: keys + values\n";
    }

    std::cout << "=== 专家：遍历缓存友好 ===\n";
    {
        std::flat_map<int, int> fm;
        for (int i = 0; i < 100; ++i) {
            fm.emplace(i, i * i);
        }
        long long sum = 0;
        for (const auto& [k, v] : fm) {
            sum += k + v;
        }
        assert(sum > 0);
        std::cout << "contiguous walk often faster than tree map\n";
    }

#else
    std::cout << "=== 本工具链无 std::flat_map：map 对照 + 平行 vector 手写模型 ===\n";
    {
        std::map<std::string, int> m{{"banana", 2}, {"apple", 1}};
        assert(m.begin()->first == "apple");

        // 手写“flat”模型：平行数组 + 有序键
        std::vector<std::string> keys{"apple", "banana"};
        std::vector<int> vals{1, 2};
        auto it = std::lower_bound(keys.begin(), keys.end(), "banana");
        assert(it != keys.end());
        const auto idx = static_cast<std::size_t>(it - keys.begin());
        assert(vals[idx] == 2);
        std::cout << "__cpp_lib_flat_map missing; emulated with parallel vectors\n";
    }
#endif

    std::cout << "[flat_map_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/flat_map_cpp23", run>;

}  // namespace
