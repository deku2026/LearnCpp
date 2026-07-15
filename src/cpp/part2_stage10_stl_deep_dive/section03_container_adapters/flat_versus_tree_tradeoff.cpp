// Topic    : flat_map/set vs 红黑树 map/set 取舍（验收点）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.2 表格（验收点）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_versus_tree_tradeoff
// Topic id : part2/stage10/section03/flat_versus_tree_tradeoff
// Refs     : https://en.cppreference.com/w/cpp/container/flat_map
//            https://en.cppreference.com/w/cpp/container/map
//            P0429 / P1222

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <version>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#include <flat_set>
#define LEARN_HAS_FLAT 1
#else
#define LEARN_HAS_FLAT 0
#endif

namespace {

template <class F>
std::int64_t time_us(F&& f) {
    const auto t0 = std::chrono::steady_clock::now();
    f();
    const auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [flat_versus_tree_tradeoff] 入门：对照表（心智模型）===\n";
    {
        const char* rows[] = {
            "layout     | tree: node heap   | flat: contiguous vector(s)",
            "lookup     | O(log n) pointer  | O(log n) binary + cache-friendly",
            "iterate    | pointer chasing   | sequential scan (fast)",
            "insert/erase | O(log n)        | O(n) element moves",
            "iter invalid | only erased     | like vector: broad invalidation",
            "memory     | high per node     | compact",
        };
        for (auto* r : rows) {
            std::cout << "  " << r << '\n';
        }
    }

    std::cout << "=== 进阶：只读点查/遍历场景的相对成本（教学计时）===\n";
    {
        constexpr int N = 20000;
        std::vector<int> keys(N);
        std::iota(keys.begin(), keys.end(), 0);
        std::mt19937 rng{123};
        std::shuffle(keys.begin(), keys.end(), rng);

        std::map<int, int> tree;
        for (int k : keys) {
            tree.emplace(k, k);
        }

#if LEARN_HAS_FLAT
        std::flat_map<int, int> flat;
        for (int k : keys) {
            flat.emplace(k, k);
        }
#else
        // 无 flat_map 时用有序 vector + lower_bound 模拟“flat 查找”
        std::vector<std::pair<int, int>> flat_vec;
        flat_vec.reserve(static_cast<std::size_t>(N));
        for (int k : keys) {
            flat_vec.emplace_back(k, k);
        }
        std::sort(flat_vec.begin(), flat_vec.end(), [](auto& a, auto& b) { return a.first < b.first; });
#endif

        volatile std::int64_t sink = 0;
        const auto us_tree_scan = time_us([&] {
            std::int64_t s = 0;
            for (const auto& [k, v] : tree) {
                s += k + v;
            }
            sink = s;
        });

#if LEARN_HAS_FLAT
        const auto us_flat_scan = time_us([&] {
            std::int64_t s = 0;
            for (const auto& [k, v] : flat) {
                s += k + v;
            }
            sink = s;
        });
#else
        const auto us_flat_scan = time_us([&] {
            std::int64_t s = 0;
            for (const auto& [k, v] : flat_vec) {
                s += k + v;
            }
            sink = s;
        });
#endif
        (void)sink;
        std::cout << "full scan N=" << N << " tree=" << us_tree_scan << "us flat/contig=" << us_flat_scan << "us\n";

        // 点查
        std::vector<int> probes = keys;
        std::shuffle(probes.begin(), probes.end(), rng);
        probes.resize(2000);

        const auto us_tree_find = time_us([&] {
            std::int64_t s = 0;
            for (int k : probes) {
                s += tree.find(k)->second;
            }
            sink = s;
        });
#if LEARN_HAS_FLAT
        const auto us_flat_find = time_us([&] {
            std::int64_t s = 0;
            for (int k : probes) {
                s += flat.find(k)->second;
            }
            sink = s;
        });
#else
        const auto us_flat_find = time_us([&] {
            std::int64_t s = 0;
            for (int k : probes) {
                auto it = std::lower_bound(flat_vec.begin(), flat_vec.end(), k,
                                           [](const auto& e, int key) { return e.first < key; });
                s += it->second;
            }
            sink = s;
        });
#endif
        std::cout << "point lookup x" << probes.size() << " tree=" << us_tree_find << "us flat=" << us_flat_find
                  << "us\n";
        assert(us_tree_scan >= 0 && us_flat_scan >= 0);
    }

    std::cout << "=== 进阶：频繁单插 — 树更合适（复杂度论证）===\n";
    {
        // 不强制计时胜负；用复杂度 + 正确性说明
        std::set<int> tree;
        for (int i = 0; i < 1000; ++i) {
            tree.insert(i);  // 每次 O(log n)，总约 O(n log n)
        }
        assert(tree.size() == 1000);

        std::vector<int> flat_keys;
        for (int i = 0; i < 1000; ++i) {
            // 模拟 flat 插入：保持有序 → 每次 O(n) 移动
            auto pos = std::lower_bound(flat_keys.begin(), flat_keys.end(), i);
            flat_keys.insert(pos, i);
        }
        assert(flat_keys.size() == 1000);
        std::cout << "n single inserts: tree O(n log n) moves nodes; "
                     "flat O(n^2) shifts\n";
    }

    std::cout << "=== 专家：决策清单 ===\n";
    {
        auto choose = [](bool many_lookups, bool rare_updates, bool need_stable_iters) -> const char* {
            if (need_stable_iters) {
                return "std::map/set (node stability)";
            }
            if (many_lookups && rare_updates) {
                return "std::flat_map/flat_set (or sorted vector)";
            }
            if (!many_lookups && !rare_updates) {
                return "std::map/set (update heavy)";
            }
            return "measure both; default map if unsure about flat availability";
        };
        assert(std::string_view(choose(true, true, false)).find("flat") != std::string_view::npos);
        assert(std::string_view(choose(true, true, true)).find("map") != std::string_view::npos);
        std::cout << "lookup+stable? → " << choose(true, true, true) << '\n';
        std::cout << "lookup+rare updates? → " << choose(true, true, false) << '\n';
        std::cout << "update heavy? → " << choose(false, false, false) << '\n';
    }

    std::cout << "=== 专家：迭代器失效差异（正确性优先于性能）===\n";
    {
        std::map<int, int> m{{1, 1}, {2, 2}};
        [[maybe_unused]] auto it = m.find(2);
        m.emplace(0, 0);
        assert(it->second == 2);  // 仍有效

#if LEARN_HAS_FLAT
        std::flat_map<int, int> fm{{1, 1}, {2, 2}};
        auto ft = fm.find(2);
        fm.emplace(0, 0);
        (void)ft;  // 插入后失效，禁止解引用
        std::cout << "tree: stable iters; flat: treat like vector\n";
#else
        std::cout << "tree iters stable; flat (when present) invalidates broadly\n";
#endif
    }

    std::cout << "[flat_versus_tree_tradeoff] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/flat_versus_tree_tradeoff", run>;

}  // namespace
