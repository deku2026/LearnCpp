// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.1 容器全表
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : sequence_associative_unordered_adapter_overview
// Topic id : part3/section02/sequence_associative_unordered_adapter_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/container
//
// 要点: 序列/关联/无序/适配器选型; 复杂度与迭代器失效口诀。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <span>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/sequence_associative_unordered_adapter_overview ===\n";

    // -------------------------------------------------------------------------
    // 序列容器
    // -------------------------------------------------------------------------
    std::vector<int> v{1, 2, 3};
    v.push_back(4);  // 尾插摊还 O(1); 可能扩容使全部迭代器失效
    assert(v.size() == 4 && v[0] == 1);

    std::array<int, 3> a{10, 20, 30};
    assert(a[1] == 20);  // 定长, 迭代器不因“扩容”失效

    std::deque<int> d{1, 2};
    d.push_front(0);
    d.push_back(3);
    assert(d.front() == 0 && d.back() == 3);

    std::list<int> lst{1, 2, 3};
    auto it = lst.begin();
    ++it;
    lst.insert(it, 99);  // 任意位置 O(1) 插, 仅涉及节点
    assert(lst.size() == 4);

    std::cout << "[intro] sequence: vector/array/deque/list ok\n";

    // -------------------------------------------------------------------------
    // 关联 / 无序
    // -------------------------------------------------------------------------
    std::map<std::string, int> ordered{{"b", 2}, {"a", 1}};
    assert(ordered.begin()->first == "a");  // 有序, 查找 O(log n)
    ordered.insert({"c", 3});

    std::set<int> s{3, 1, 2};
    assert(*s.begin() == 1);

    std::unordered_map<std::string, int> um{{"x", 1}, {"y", 2}};
    assert(um["x"] == 1);  // 平均 O(1); rehash 使迭代器失效

    std::cout << "[advanced] map/set ordered; unordered_map hashed\n";

    // -------------------------------------------------------------------------
    // 适配器 + span 视图
    // -------------------------------------------------------------------------
    std::stack<int> st;
    st.push(1);
    st.push(2);
    assert(st.top() == 2);
    st.pop();
    assert(st.top() == 1);

    std::queue<int> q;
    q.push(10);
    q.push(20);
    assert(q.front() == 10);
    q.pop();
    assert(q.front() == 20);

    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(10);
    pq.push(7);
    assert(pq.top() == 10);  // 默认最大堆

    std::span<const int> sp{v};
    assert(sp.size() == v.size() && sp[0] == 1);

    std::cout << "[expert] adapters stack/queue/priority_queue; span non-owning\n";
    std::cout << "invalidation: vector grow invalidates all; node-based erase "
                 "only killed element; unordered rehash invalidates iterators\n";
    std::cout << "sequence_associative_unordered_adapter_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/sequence_associative_unordered_adapter_overview", run>;

}  // namespace
