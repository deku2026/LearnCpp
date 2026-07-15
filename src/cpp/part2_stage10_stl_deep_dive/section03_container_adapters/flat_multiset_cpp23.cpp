// Topic    : C++23 std::flat_multiset
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.2（🆕）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multiset_cpp23
// Topic id : part2/stage10/section03/flat_multiset_cpp23
// Refs     : https://en.cppreference.com/w/cpp/container/flat_multiset
//            feature-test: __cpp_lib_flat_set

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <version>

#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
#include <flat_set>
#define LEARN_HAS_FLAT_MULTISET 1
#else
#define LEARN_HAS_FLAT_MULTISET 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [flat_multiset_cpp23] 入门：有序可重复，连续存储 ===\n";

#if LEARN_HAS_FLAT_MULTISET
    {
        std::flat_multiset<int> fms{1, 2, 2, 2, 3};
        assert(fms.size() == 5);
        assert(fms.count(2) == 3);
        auto [lo, hi] = fms.equal_range(2);
        assert(static_cast<std::size_t>(hi - lo) == 3);
        std::vector<int> got(fms.begin(), fms.end());
        assert((got == std::vector<int>{1, 2, 2, 2, 3}));
        std::cout << "flat_multiset keeps duplicates in sorted order\n";
    }

    std::cout << "=== 进阶：insert 总成功；erase(key) 删全部等价 ===\n";
    {
        std::flat_multiset<int> fms;
        fms.insert(5);
        fms.insert(5);
        fms.insert(1);
        assert(fms.count(5) == 2);
        fms.erase(5);
        assert(fms.count(5) == 0);
        assert(fms.size() == 1);
        std::cout << "same multi-key rules as multiset, array layout\n";
    }

    std::cout << "=== 专家：sorted_equivalent 批量构造 ===\n";
    {
        std::vector<int> raw{1, 1, 2, 3, 3, 3};
        std::flat_multiset<int> fms{std::sorted_equivalent, raw.begin(), raw.end()};
        assert(fms.size() == 6);
        assert(fms.count(3) == 3);
        std::cout << "sorted_equivalent for already-ordered multi input\n";
    }

#else
    std::cout << "=== 本工具链无 flat_multiset：multiset 对照 ===\n";
    {
        std::multiset<int> ms{1, 2, 2, 2, 3};
        assert(ms.count(2) == 3);
        std::vector<int> got(ms.begin(), ms.end());
        assert((got == std::vector<int>{1, 2, 2, 2, 3}));
        std::cout << "emulate: multiset interface, flat uses contiguous keys\n";
    }
#endif

    std::cout << "=== 专家：适用 — 多键范围查询且读多写少 ===\n";
    std::cout << "equal_range on contiguous keys is cache-friendly\n";

    std::cout << "[flat_multiset_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/flat_multiset_cpp23", run>;

}  // namespace
