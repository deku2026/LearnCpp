// Topic    : std::list：双向链表（给定迭代器处 O(1) 插删）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.3
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_list
// Topic id : part2/stage10/section01/std_list
// Refs     : https://en.cppreference.com/w/cpp/container/list
//            ISO [list]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_list] 入门：双向链表基本操作 ===\n";
    {
        std::list<int> lst{1, 2, 3};
        lst.push_front(0);
        lst.push_back(4);
        assert(lst.front() == 0 && lst.back() == 4);
        assert(lst.size() == 5);

        // 双向：可 --
        auto it = lst.end();
        --it;
        assert(*it == 4);
        std::cout << "push front/back + bidirectional walk\n";
    }

    std::cout << "=== 入门：任意位置 O(1) 插删（已有迭代器）===\n";
    {
        std::list<int> lst{1, 2, 4};
        auto it = lst.begin();
        std::advance(it, 2);  // 指向 4；advance 是 O(n)
        lst.insert(it, 3);    // 插入本身 O(1)
        assert((lst == std::list<int>{1, 2, 3, 4}));

        it = lst.begin();
        std::advance(it, 1);
        it = lst.erase(it);  // 删 2，返回下一元素
        assert(*it == 3);
        assert((lst == std::list<int>{1, 3, 4}));
        std::cout << "insert/erase O(1) once iterator is known\n";
    }

    std::cout << "=== 进阶：迭代器/引用不因插删失效（除被删）===\n";
    {
        std::list<int> lst{10, 20, 30};
        auto it20 = std::next(lst.begin());
        [[maybe_unused]] int& r30 = lst.back();
        lst.push_front(0);
        lst.push_back(40);
        lst.insert(it20, 15);
        // it20 / r30 仍有效
        assert(*it20 == 20);
        assert(r30 == 30);
        lst.erase(lst.begin());  // 删 0，不影响 it20
        assert(*it20 == 20);
        std::cout << "node-based: other iterators stay valid\n";
    }

    std::cout << "=== 进阶：splice / merge / sort 成员 ===\n";
    {
        std::list<int> a{1, 3, 5};
        std::list<int> b{2, 4, 6};
        a.merge(b);  // 要求两边有序；b 被掏空
        assert(b.empty());
        assert((a == std::list<int>{1, 2, 3, 4, 5, 6}));

        std::list<int> c{3, 1, 2};
        c.sort();  // list 不能 std::sort（非随机访问）
        assert((c == std::list<int>{1, 2, 3}));

        std::list<int> d{1, 2, 3};
        std::list<int> e{10, 20};
        d.splice(std::next(d.begin()), e);  // 整表接到 d 的位置，O(1)
        assert(e.empty());
        assert((d == std::list<int>{1, 10, 20, 2, 3}));
        std::cout << "merge/sort/splice are list specialties\n";
    }

    std::cout << "=== 专家：缓存差 + 何时才该用 list ===\n";
    {
        // 遍历靠指针追逐，缓存不友好；“插入 O(1)”常被 advance O(n)+常数吃掉
        // 适用：元素很大、频繁在已知迭代器处插删、且必须保持其他迭代器有效
        //       或需要 splice 常数时间拼接
        std::list<std::string> big_nodes;
        big_nodes.emplace_back(100, 'x');
        [[maybe_unused]] auto keep = big_nodes.begin();
        big_nodes.emplace_front(50, 'y');
        assert(keep->size() == 100);
        std::cout << "default prefer vector; list only with measured need\n";
    }

    std::cout << "=== 专家：remove/unique 成员 vs 算法 ===\n";
    {
        std::list<int> lst{1, 2, 2, 3, 2, 4};
        lst.remove(2);  // 真正删除所有 2
        assert((lst == std::list<int>{1, 3, 4}));
        lst = {1, 1, 2, 2, 2, 3};
        lst.unique();  // 压相邻重复
        assert((lst == std::list<int>{1, 2, 3}));
        std::cout << "list::remove/unique erase nodes (unlike std::remove)\n";
    }

    std::cout << "[std_list] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/std_list", run>;

}  // namespace
