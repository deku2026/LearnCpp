// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 悬垂迭代器)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_iterator
// Topic id : part6/b/section03/dangling_iterator
//
// 要点: 容器 reallocation/erase 使迭代器失效——同一类「借用先于所有者死亡」。
// 参考: vector iterator invalidation

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling iterator (safe contrasts) ===\n";

    // vector：扩容使所有迭代器/引用/指针失效
    {
        std::vector<int> v{1, 2, 3};
        auto it = v.begin();
        assert(*it == 1);

        v.reserve(v.capacity() + 1);
        v.push_back(4);
        // 不使用旧 it！
        it = v.begin();  // 重新获取
        assert(*it == 1);
        assert(v.back() == 4);
    }

    // erase 后用返回的下一迭代器
    {
        std::vector<int> v{1, 2, 3, 4};
        for (auto it = v.begin(); it != v.end();) {
            if (*it % 2 == 0) {
                it = v.erase(it);  // erase 返回下一有效迭代器
            } else {
                ++it;
            }
        }
        assert((v == std::vector<int>{1, 3}));
    }

    // list：erase 只使被删迭代器失效，其它仍有效
    {
        std::list<int> L{10, 20, 30};
        auto a = L.begin();
        auto b = std::next(a);
        auto c = std::next(b);
        L.erase(b);  // b 失效；a、c 仍有效
        assert(*a == 10);
        assert(*c == 30);
        assert(L.size() == 2);
    }

    // 移动后源容器迭代器通常失效
    {
        std::vector<int> a{1, 2, 3};
        std::vector<int> b = std::move(a);
        // 不使用指向 a 的旧迭代器
        assert(b.size() == 3);
        assert(b[0] == 1);
    }

    std::cout << "  after mutation that invalidates: re-acquire iterators\n";
    std::cout << "dangling_iterator: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_iterator", run>;

}  // namespace
