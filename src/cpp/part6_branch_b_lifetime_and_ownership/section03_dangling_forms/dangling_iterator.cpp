// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 悬垂迭代器)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_iterator
// Topic id : part6/b/section03/dangling_iterator
//
// 要点: reallocation/erase 使迭代器失效——借用先于所有者「逻辑死亡」。
// 参考: container iterator invalidation rules

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling iterator (safe contrasts) ===\n";

    // --- 入门: vector 扩容使全部迭代器/引用/指针失效 ---
    {
        std::vector<int> v{1, 2, 3};
        auto it = v.begin();
        assert(*it == 1);
        v.reserve(v.capacity() + 1);
        v.push_back(4);
        // 不使用旧 it
        it = v.begin();
        assert(*it == 1);
        assert(v.back() == 4);
    }

    // erase 使用返回的下一迭代器
    {
        std::vector<int> v{1, 2, 3, 4};
        for (auto it = v.begin(); it != v.end();) {
            if (*it % 2 == 0)
                it = v.erase(it);
            else
                ++it;
        }
        assert((v == std::vector<int>{1, 3}));
    }

    // --- 进阶: list 与 vector 失效规则不同 ---
    {
        std::list<int> L{10, 20, 30};
        auto a = L.begin();
        auto b = std::next(a);
        auto c = std::next(b);
        L.erase(b);  // 仅 b 失效
        assert(*a == 10);
        assert(*c == 30);
        assert(L.size() == 2);
    }

    // deque: 中间插入可能使所有迭代器失效（实现相关，保守重取）
    {
        std::deque<int> d{1, 2, 3};
        d.push_back(4);
        auto it = d.begin();
        assert(*it == 1);
        d.push_front(0);
        it = d.begin();  // 重取
        assert(*it == 0);
    }

    // 移动后源容器迭代器通常失效
    {
        std::vector<int> a{1, 2, 3};
        std::vector<int> b = std::move(a);
        assert(b.size() == 3 && b[0] == 1);
    }

    // --- 专家 ---
    std::cout << "  after invalidating mutation: re-acquire iterators\n";
    std::cout << "  iterator = borrow into container storage (unified dangling model)\n";
    std::cout << "dangling_iterator: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_iterator", run>;

}  // namespace
