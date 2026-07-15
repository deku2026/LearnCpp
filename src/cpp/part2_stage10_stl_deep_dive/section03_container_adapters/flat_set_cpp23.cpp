// Topic    : C++23 std::flat_set：有序 vector 适配的 set
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.2（🆕）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_set_cpp23
// Topic id : part2/stage10/section03/flat_set_cpp23
// Refs     : https://en.cppreference.com/w/cpp/container/flat_set
//            P1222R4
//            feature-test: __cpp_lib_flat_set

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <version>

#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
#include <flat_set>
#define LEARN_HAS_FLAT_SET 1
#else
#define LEARN_HAS_FLAT_SET 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [flat_set_cpp23] 入门：接口像 set，底层连续 ===\n";

#if LEARN_HAS_FLAT_SET
    {
        std::flat_set<int> fs{3, 1, 4, 1, 5};
        assert(fs.size() == 4);  // 去重
        assert(fs.contains(4));
        // 有序遍历 + 连续内存
        std::vector<int> got(fs.begin(), fs.end());
        assert((got == std::vector<int>{1, 3, 4, 5}));
        std::cout << "flat_set: unique sorted, contiguous keys\n";
    }

    std::cout << "=== 进阶：insert/erase O(n)，查找 O(log n) 二分 ===\n";
    {
        std::flat_set<int> fs;
        fs.insert(10);
        fs.insert(5);
        fs.insert(20);
        auto it = fs.find(5);
        assert(it != fs.end() && *it == 5);
        fs.erase(10);
        assert(!fs.contains(10));
        assert(fs.size() == 2);
        std::cout << "binary search lookup; shift on insert/erase\n";
    }

    std::cout << "=== 进阶：sorted_unique 从已排序数据高效构造 ===\n";
    {
        std::vector<int> raw{1, 2, 3, 5, 8};
        std::flat_set<int> fs{std::sorted_unique, raw.begin(), raw.end()};
        assert(fs.size() == 5);
        assert(fs.contains(8));
        std::cout << "sorted_unique skips sort when input already ordered\n";
    }

    std::cout << "=== 专家：extract 底层容器 / 与 set 失效对比 ===\n";
    {
        std::flat_set<int> fs{1, 2, 3};
        auto keys = std::move(fs).extract();  // 取出底层 sequence
        assert(keys.size() == 3);
        // 插入删除会使迭代器类似 vector 一样大面积失效
        std::flat_set<int> fs3{1, 2, 3, 4};
        auto it = fs3.begin();
        fs3.insert(0);  // 可能移动全部
        (void)it;       // 已失效，不可用
        std::cout << "extract() yields underlying container; iters fragile\n";
    }

#else
    std::cout << "=== 本工具链无 std::flat_set：用 set + 说明替代语义 ===\n";
    {
        std::set<int> s{3, 1, 4, 1, 5};
        assert(s.size() == 4);
        // 语义对照：flat_set 提供相同有序唯一接口，但内存连续、插删 O(n)
        std::vector<int> as_vec(s.begin(), s.end());
        assert((as_vec == std::vector<int>{1, 3, 4, 5}));
        std::cout << "__cpp_lib_flat_set not available; showed set equivalent\n";
    }
#endif

    std::cout << "=== 专家：何时选 flat_set ===\n";
    std::cout << "read/lookup heavy + rare inserts → flat_set; "
                 "frequent node-stable updates → set\n";

    std::cout << "[flat_set_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/flat_set_cpp23", run>;

}  // namespace
