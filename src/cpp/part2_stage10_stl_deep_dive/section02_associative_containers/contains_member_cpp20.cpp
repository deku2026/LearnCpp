// Topic    : C++20 contains()：布尔存在性查询
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.4
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : contains_member_cpp20
// Topic id : part2/stage10/section02/contains_member_cpp20
// Refs     : https://en.cppreference.com/w/cpp/container/map/contains
//            https://en.cppreference.com/w/cpp/container/unordered_map/contains
//            https://en.cppreference.com/w/cpp/container/set/contains
//            P0458R2

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [contains_member_cpp20] 入门：替代 find != end ===\n";
    {
        std::map<std::string, int> m{{"a", 1}, {"b", 2}};
        // C++17 以前
        [[maybe_unused]] const bool old_way = m.find("a") != m.end();
        // C++20
        [[maybe_unused]] const bool new_way = m.contains("a");
        assert(old_way && new_way);
        assert(!m.contains("z"));
        std::cout << "m.contains(key) == (m.find(key) != m.end())\n";
    }

    std::cout << "=== 入门：set / unordered_* 同样具备 ===\n";
    {
        std::set<int> s{1, 2, 3};
        std::unordered_set<int> us{4, 5, 6};
        std::unordered_map<int, char> um{{1, 'x'}};
        assert(s.contains(2));
        assert(us.contains(5));
        assert(um.contains(1));
        assert(!um.contains(9));
        std::cout << "contains is on all associative / unordered associative\n";
    }

    std::cout << "=== 进阶：与 count 的语义差 ===\n";
    {
        std::map<int, int> m{{1, 10}};
        std::multiset<int> ms{1, 1, 2};
        // map/set：count 是 0/1；contains 更表达“是否存在”
        assert(m.count(1) == 1 && m.contains(1));
        assert(m.count(2) == 0 && !m.contains(2));
        // multi*：count 可为 >1；contains 仍是 bool
        assert(ms.count(1) == 2 && ms.contains(1));
        std::cout << "contains → bool; count → cardinality (esp. multi*)\n";
    }

    std::cout << "=== 进阶：透明比较器下的异质 contains ===\n";
    {
        std::map<std::string, int, std::less<>> m{{"alpha", 1}};
        assert(m.contains(std::string_view{"alpha"}));
        assert(m.contains("alpha"));
        assert(!m.contains(std::string_view{"beta"}));
        std::cout << "contains participates in heterogeneous lookup\n";
    }

    std::cout << "=== 专家：序列容器没有 contains 成员（用算法）===\n";
    {
        std::vector<int> v{1, 2, 3};
        // v.contains(2);  // ❌ 不存在
        [[maybe_unused]] const bool ok = std::ranges::contains(v, 2);  // C++23 算法
        assert(ok);
        [[maybe_unused]] const bool old = std::find(v.begin(), v.end(), 2) != v.end();
        assert(old);
        std::cout << "sequence: ranges::contains / find; assoc: member contains\n";
    }

    std::cout << "=== 专家：可读性 — 意图直接是存在性 ===\n";
    {
        std::unordered_map<std::string, int> cfg{{"debug", 1}, {"jobs", 4}};
        if (cfg.contains("debug")) {
            assert(cfg["debug"] == 1);
        }
        // 注意：cfg["missing"] 会插入；先 contains 再 at 更安全
        if (!cfg.contains("missing")) {
            // 不要写 cfg["missing"] 除非想插入默认值
            assert(true);
        }
        try {
            (void)cfg.at("missing");
            assert(false);
        } catch (const std::out_of_range&) {
            std::cout << "contains then at: no accidental insertion\n";
        }
    }

    std::cout << "[contains_member_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02/contains_member_cpp20", run>;

}  // namespace
