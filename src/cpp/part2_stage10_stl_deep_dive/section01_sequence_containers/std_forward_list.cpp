// Topic    : std::forward_list：单链表（最省内存的节点式容器）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.3
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_forward_list
// Topic id : part2/stage10/section01/std_forward_list
// Refs     : https://en.cppreference.com/w/cpp/container/forward_list
//            ISO [forward.list]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <iterator>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_forward_list] 入门：单向 + before_begin API ===\n";
    {
        std::forward_list<int> fl{2, 3, 4};
        fl.push_front(1);
        assert(fl.front() == 1);
        // 无 size()（C++ 标准：O(n) 才可算长度，故不提供常数 size）
        assert(std::distance(fl.begin(), fl.end()) == 4);

        // 在头部插入用 before_begin
        fl.insert_after(fl.before_begin(), 0);
        assert(fl.front() == 0);
        std::cout << "no size(); insert via before_begin / insert_after\n";
    }

    std::cout << "=== 入门：insert_after / erase_after ===\n";
    {
        std::forward_list<int> fl{1, 3, 4};
        auto it = fl.begin();  // 1
        fl.insert_after(it, 2);
        assert((fl == std::forward_list<int>{1, 2, 3, 4}));

        // 删除 it 之后的元素
        fl.erase_after(it);  // 删 2
        assert((fl == std::forward_list<int>{1, 3, 4}));
        std::cout << "mutate the *next* of a known iterator\n";
    }

    std::cout << "=== 进阶：只能前向，不能 -- ===\n";
    {
        std::forward_list<int> fl{10, 20, 30};
        auto it = fl.begin();
        ++it;
        assert(*it == 20);
        // --it;  // ❌ 无双向
        // 找“前驱”只能从头再走，O(n)
        auto pred = fl.before_begin();
        for (auto i = fl.begin(); i != it; ++i) {
            ++pred;
        }
        fl.erase_after(pred);
        assert((fl == std::forward_list<int>{10, 30}));
        std::cout << "forward_iterator only; erase needs predecessor\n";
    }

    std::cout << "=== 进阶：splice_after / remove / reverse ===\n";
    {
        std::forward_list<int> a{1, 2};
        std::forward_list<int> b{3, 4};
        a.splice_after(a.begin(), b);  // 把 b 接到 a 的第一个元素后
        assert(b.empty());
        assert((a == std::forward_list<int>{1, 3, 4, 2}));

        a.remove(3);
        assert((a == std::forward_list<int>{1, 4, 2}));
        a.reverse();
        assert((a == std::forward_list<int>{2, 4, 1}));
        a.sort();
        assert((a == std::forward_list<int>{1, 2, 4}));
        std::cout << "splice_after/remove/reverse/sort members\n";
    }

    std::cout << "=== 专家：内存更省 vs list，失效规则同节点式 ===\n";
    {
        std::forward_list<int> fl{1, 2, 3};
        [[maybe_unused]] auto keep = std::next(fl.begin());  // 2
        fl.push_front(0);
        fl.insert_after(fl.begin(), 9);
        assert(*keep == 2);  // 其他节点迭代器不失效

        // 比 list 少一个反向指针，节点开销更低；接口也更“别扭”
        // 适用：只要前向遍历、极端省内存、已知前驱位置频繁插删
        std::cout << "cheaper nodes than list; prefer vector unless need "
                     "stable node iterators + forward-only\n";
    }

    std::cout << "=== 专家：清空与 before_begin 永有效 ===\n";
    {
        std::forward_list<int> fl{1, 2};
        fl.clear();
        assert(fl.empty());
        // before_begin 始终可解引用语义上作为“头前哨”，insert_after 可用
        fl.insert_after(fl.before_begin(), 42);
        assert(fl.front() == 42);
        std::cout << "before_begin is a durable past-the-front sentinel\n";
    }

    std::cout << "[std_forward_list] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/std_forward_list", run>;

}  // namespace
