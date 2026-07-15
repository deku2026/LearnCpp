// Topic    : C++23 std::flat_multimap
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.2（🆕）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multimap_cpp23
// Topic id : part2/stage10/section03/flat_multimap_cpp23
// Refs     : https://en.cppreference.com/w/cpp/container/flat_multimap
//            feature-test: __cpp_lib_flat_map

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <version>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#define LEARN_HAS_FLAT_MULTIMAP 1
#else
#define LEARN_HAS_FLAT_MULTIMAP 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [flat_multimap_cpp23] 入门：多值有序 + 连续 ===\n";

#if LEARN_HAS_FLAT_MULTIMAP
    {
        std::flat_multimap<std::string, int> fmm;
        fmm.emplace("alice", 90);
        fmm.emplace("bob", 80);
        fmm.emplace("alice", 95);
        assert(fmm.count("alice") == 2);
        assert(fmm.size() == 3);

        int sum = 0;
        auto [lo, hi] = fmm.equal_range("alice");
        for (auto it = lo; it != hi; ++it) {
            sum += it->second;
        }
        assert(sum == 185);
        std::cout << "flat_multimap: multi values, sorted keys contiguous\n";
    }

    std::cout << "=== 进阶：无 operator[]；用 equal_range / insert ===\n";
    {
        std::flat_multimap<int, char> fmm{{1, 'a'}, {1, 'b'}, {2, 'c'}};
        fmm.insert({1, 'z'});
        assert(fmm.count(1) == 3);
        fmm.erase(1);
        assert(fmm.count(1) == 0);
        assert(fmm.size() == 1);
        std::cout << "same API shape as multimap\n";
    }

    std::cout << "=== 专家：sorted_equivalent 构建 ===\n";
    {
        std::vector<std::pair<int, int>> kvs{{1, 10}, {1, 11}, {2, 20}, {3, 30}};
        std::flat_multimap<int, int> fmm{std::sorted_equivalent, kvs.begin(), kvs.end()};
        assert(fmm.count(1) == 2);
        std::cout << "bulk load from sorted multi key sequence\n";
    }

#else
    std::cout << "=== 本工具链无 flat_multimap：multimap 对照 ===\n";
    {
        std::multimap<std::string, int> mm;
        mm.emplace("alice", 90);
        mm.emplace("alice", 95);
        assert(mm.count("alice") == 2);
        std::cout << "emulate: multimap; flat_multimap packs into vectors\n";
    }
#endif

    std::cout << "=== 专家：写少读多的倒排/事件表 ===\n";
    std::cout << "many values per key + scan equal_range → prefer flat multi*\n";

    std::cout << "[flat_multimap_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/flat_multimap_cpp23", run>;

}  // namespace
