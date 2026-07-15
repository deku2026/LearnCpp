// Topic    : std::priority_queue：堆（优先队列）适配器
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_priority_queue
// Topic id : part2/stage10/section03/std_priority_queue
// Refs     : https://en.cppreference.com/w/cpp/container/priority_queue
//            https://en.cppreference.com/w/cpp/algorithm/make_heap
//            ISO [priority.queue]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_priority_queue] 入门：默认最大堆 ===\n";
    {
        std::priority_queue<int> pq;
        for (int x : {3, 1, 4, 1, 5, 9, 2}) {
            pq.push(x);
        }
        assert(pq.top() == 9);
        pq.pop();
        assert(pq.top() == 5);
        std::cout << "default: largest element at top\n";
    }

    std::cout << "=== 入门：最小堆 — greater<> ===\n";
    {
        std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
        for (int x : {3, 1, 4, 1, 5}) {
            min_pq.push(x);
        }
        assert(min_pq.top() == 1);
        min_pq.pop();
        assert(min_pq.top() == 1);
        min_pq.pop();
        assert(min_pq.top() == 3);
        std::cout << "priority_queue<T, vector<T>, greater<T>> is min-heap\n";
    }

    std::cout << "=== 进阶：自定义优先级（任务调度）===\n";
    {
        struct Task {
            int priority = 0;
            std::string name;
        };
        auto cmp = [](const Task& a, const Task& b) {
            return a.priority < b.priority;  // 大 priority 先出
        };
        std::priority_queue<Task, std::vector<Task>, decltype(cmp)> pq(cmp);
        pq.push(Task{1, "low"});
        pq.push(Task{5, "high"});
        pq.push(Task{3, "mid"});
        assert(pq.top().name == "high");
        pq.pop();
        assert(pq.top().name == "mid");
        std::cout << "custom comparator defines urgency\n";
    }

    std::cout << "=== 进阶：与 make_heap 关系（底层就是堆算法）===\n";
    {
        std::vector<int> v{3, 1, 4, 1, 5};
        std::ranges::make_heap(v);
        assert(v.front() == 5);
        std::ranges::pop_heap(v);
        v.pop_back();
        assert(v.front() == 4);
        // priority_queue 把这些封装成 push/pop/top
        std::cout << "adapter over heap algorithms on random-access container\n";
    }

    std::cout << "=== 专家：无迭代器；不能高效任意删中间 ===\n";
    {
        std::priority_queue<int> pq;
        pq.push(2);
        pq.push(8);
        pq.push(5);
        // 只能看 top；要“提高某元素优先级”需换结构（如 set + handle）
        std::vector<int> drained;
        while (!pq.empty()) {
            drained.push_back(pq.top());
            pq.pop();
        }
        assert((drained == std::vector<int>{8, 5, 2}));
        std::cout << "no decrease-key API; use set/policy-based or diy heap\n";
    }

    std::cout << "=== 专家：pair 作元素时的字典序堆 ===\n";
    {
        using Item = std::pair<int, std::string>;
        std::priority_queue<Item> pq;  // 先比 first
        pq.emplace(2, "b");
        pq.emplace(3, "a");
        pq.emplace(2, "c");
        assert(pq.top().first == 3);
        pq.pop();
        // first 同为 2 时按 string 较大者
        assert(pq.top().second == "c");
        std::cout << "pair orders by first then second\n";
    }

    std::cout << "[std_priority_queue] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/std_priority_queue", run>;

}  // namespace
