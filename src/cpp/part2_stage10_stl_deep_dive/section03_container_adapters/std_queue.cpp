// Topic    : std::queue：FIFO 容器适配器
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_queue
// Topic id : part2/stage10/section03/std_queue
// Refs     : https://en.cppreference.com/w/cpp/container/queue
//            ISO [queue]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_queue] 入门：FIFO push/pop/front/back ===\n";
    {
        std::queue<int> q;
        assert(q.empty());
        q.push(1);
        q.push(2);
        q.push(3);
        assert(q.size() == 3);
        assert(q.front() == 1);
        assert(q.back() == 3);
        q.pop();  // 弹出 front
        assert(q.front() == 2);
        std::cout << "queue: first-in first-out\n";
    }

    std::cout << "=== 进阶：底层默认 deque，可换 list（勿用 vector）===\n";
    {
        // vector 没有 push_front/pop_front，不能做 queue 底层
        std::queue<std::string, std::list<std::string>> q;
        q.emplace("job-a");
        q.emplace("job-b");
        assert(q.front() == "job-a");
        q.pop();
        assert(q.front() == "job-b");
        std::cout << "Container must support front/back/push_back/pop_front\n";
    }

    std::cout << "=== 进阶：简易任务队列模拟 ===\n";
    {
        std::queue<std::string> tasks;
        for (auto* t : {"parse", "typecheck", "codegen", "link"}) {
            tasks.push(t);
        }
        std::vector<std::string> done;
        while (!tasks.empty()) {
            done.push_back(tasks.front());
            tasks.pop();
        }
        assert(done.size() == 4);
        assert(done.front() == "parse" && done.back() == "link");
        std::cout << "BFS/work-queue pattern\n";
    }

    std::cout << "=== 专家：与 deque 直接用的取舍 ===\n";
    {
        // queue 隐藏迭代器/随机访问，强制 FIFO 纪律
        // 需要偷看中间、迭代 → 直接用 deque
        std::deque<int> d{1, 2, 3};
        assert(d[1] == 2);  // deque 可以；queue 不行
        std::queue<int> q(d);
        assert(q.size() == 3 && q.front() == 1);
        std::cout << "adapter enforces interface; raw deque is more open\n";
    }

    std::cout << "=== 专家：比较与 emplace ===\n";
    {
        std::queue<int> a, b;
        a.push(1);
        a.emplace(2);
        b.push(1);
        b.push(2);
        assert(a == b);
        std::cout << "emplace constructs in place at back\n";
    }

    std::cout << "[std_queue] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/std_queue", run>;

}  // namespace
