// Topic    : 各容器迭代器失效规则总表（高频 bug 源）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 4.2（验收点）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : per_container_invalidation_rules
// Topic id : part2/stage10/section04/per_container_invalidation_rules
// Refs     : https://en.cppreference.com/w/cpp/container
//            各容器页 Iterator invalidation 小节
//            ISO [container.requirements]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [per_container_invalidation_rules] 入门：总表 ===\n";
    {
        const char* table[] = {
            "vector: realloc→ALL; else insert/erase after pos",
            "deque: mid insert/erase→ALL; ends invalidate iters (refs ok)",
            "list/forward_list: only erased element",
            "map/set/multi*: only erased element",
            "unordered_*: rehash→all iterators (refs/pointers stay)",
        };
        for (auto* row : table) {
            std::cout << "  - " << row << '\n';
        }
    }

    std::cout << "=== 入门：vector — 扩容全失效 ===\n";
    {
        std::vector<int> v{1, 2};
        v.reserve(2);
        int* p = v.data();
        v.push_back(3);  // 超 capacity
        assert(v.data() != p);
        std::cout << "vector reallocation moves storage → old iters dead\n";
    }

    std::cout << "=== 进阶：deque — 头尾插 vs 中间插 ===\n";
    {
        std::deque<int> d{1, 2, 3};
        int& r = d[1];    // 指向 2
        d.push_front(0);  // 头插：迭代器失效，引用通常仍有效
        assert(r == 2);
        d.insert(d.begin() + 2, 99);  // 中间：引用也失效（标准：全部失效）
        // 不要再使用 r
        assert(d[2] == 99);
        std::cout << "deque end ops keep refs; middle ops kill all\n";
    }

    std::cout << "=== 进阶：list / map — 节点式稳定 ===\n";
    {
        std::list<int> lst{1, 2, 3};
        auto it = std::next(lst.begin());
        lst.push_front(0);
        lst.push_back(4);
        lst.insert(it, 9);
        assert(*it == 2);

        std::map<int, std::string> m{{1, "a"}, {2, "b"}};
        auto mit = m.find(2);
        m.emplace(0, "z");
        m.erase(1);
        assert(mit->second == "b");
        std::cout << "node-based: other iterators remain valid\n";
    }

    std::cout << "=== 进阶：unordered — rehash 只废迭代器 ===\n";
    {
        std::unordered_map<int, int> um;
        um.reserve(2);
        um[1] = 10;
        int& r = um[1];
        // 迫使 rehash
        for (int i = 2; i < 100; ++i) {
            um[i] = i;
        }
        assert(r == 10);  // 引用仍有效
        // 旧 iterator 不可用；重新 find
        assert(um.find(1)->second == 10);
        std::cout << "rehash: iterators dead, references to elements live\n";
    }

    std::cout << "=== 专家：边遍历边删 — 统一正确模式 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        for (auto it = v.begin(); it != v.end();) {
            if (*it % 2 == 0) {
                it = v.erase(it);
            } else {
                ++it;
            }
        }
        assert((v == std::vector<int>{1, 3, 5}));

        std::map<int, int> m{{1, 1}, {2, 2}, {3, 3}, {4, 4}};
        for (auto it = m.begin(); it != m.end();) {
            if (it->first % 2 == 0) {
                it = m.erase(it);  // C++11+ map::erase returns next
            } else {
                ++it;
            }
        }
        assert(m.size() == 2 && m.contains(1) && m.contains(3));

        // C++20  sequential erase_if
        std::vector<int> w{1, 2, 3, 4};
        std::erase_if(w, [](int x) { return x > 2; });
        assert((w == std::vector<int>{1, 2}));
        std::cout << "erase return value or erase_if; never ++ invalidated it\n";
    }

    std::cout << "=== 专家：string 近似 vector 的失效（简记）===\n";
    {
        std::string s = "hi";
        s.reserve(2);
        char* p = s.data();
        s.push_back('!');
        // SSO/实现相关：小字符串可能不在堆；但 capacity 增长语义同 vector
        if (s.capacity() > 2) {
            // 可能 reallocate
        }
        assert(s == "hi!");
        (void)p;
        std::cout << "basic_string follows vector-like invalidation rules\n";
    }

    std::cout << "[per_container_invalidation_rules] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section04/per_container_invalidation_rules", run>;

}  // namespace
