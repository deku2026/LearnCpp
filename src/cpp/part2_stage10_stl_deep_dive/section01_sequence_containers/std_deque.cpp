// Topic    : std::deque：双端队列（头尾 O(1)，分段连续）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.3
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_deque
// Topic id : part2/stage10/section01/std_deque
// Refs     : https://en.cppreference.com/w/cpp/container/deque
//            ISO [deque]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_deque] 入门：头尾插入都摊还 O(1) ===\n";
    {
        std::deque<int> d;
        d.push_back(2);
        d.push_back(3);
        d.push_front(1);
        d.push_front(0);
        assert((d == std::deque<int>{0, 1, 2, 3}));
        assert(d.front() == 0 && d.back() == 3);

        d.pop_front();
        d.pop_back();
        assert((d == std::deque<int>{1, 2}));
        std::cout << "push/pop front+back without shifting whole body\n";
    }

    std::cout << "=== 入门：随机访问 operator[] / at ===\n";
    {
        std::deque<int> d{10, 20, 30, 40};
        assert(d[2] == 30);
        assert(d.at(1) == 20);
        try {
            (void)d.at(99);
            assert(false);
        } catch (const std::out_of_range&) {
            std::cout << "at throws on OOB\n";
        }
    }

    std::cout << "=== 进阶：中间 insert/erase 是 O(n)，头尾引用语义特殊 ===\n";
    {
        std::deque<int> d{1, 2, 3, 4};
        d.insert(d.begin() + 2, 99);  // 中间插入
        assert(d[2] == 99);
        d.erase(d.begin() + 2);
        assert((d == std::deque<int>{1, 2, 3, 4}));

        // 头尾插入：迭代器失效，但引用/指针对元素本身通常仍有效（标准：头尾插入
        // 使所有迭代器失效，引用不失效；中间插入使全部失效）
        [[maybe_unused]] int& r = d.front();
        d.push_front(0);
        assert(r == 1);  // front 元素引用在头插后仍指原元素
        assert(d.front() == 0);
        std::cout << "mid ops O(n); front/back insert keeps element refs\n";
    }

    std::cout << "=== 进阶：作 queue 底层、滑动窗口缓冲 ===\n";
    {
        std::deque<std::string> window;
        for (const char* s : {"a", "b", "c", "d"}) {
            window.emplace_back(s);
            if (window.size() > 3) {
                window.pop_front();
            }
        }
        assert(window.size() == 3);
        assert(window.front() == "b" && window.back() == "d");
        std::cout << "sliding window via push_back + pop_front\n";
    }

    std::cout << "=== 专家：vs vector 的缓存与接口差异 ===\n";
    {
        std::deque<int> d(100, 1);
        // 无 data()、非真正连续内存：不能当 C 数组传给要连续缓冲的 API
        // 迭代器是随机访问，但 ++ 可能跨块，缓存局部性弱于 vector
        assert(std::distance(d.begin(), d.end()) == 100);
        assert(std::ranges::all_of(d, [](int x) { return x == 1; }));
        std::cout << "random-access but chunked; no data(); prefer vector "
                     "unless need front ops\n";
    }

    std::cout << "=== 专家：迭代器失效摘要 ===\n";
    // insert 中间 / erase 中间 → 全部 iterator/ref 失效
    // push/pop 头或尾 → 所有 iterator 失效；refs 对未删元素可保留
    // clear → 全部失效
    std::cout << "deque invalidation stricter than vector on end inserts "
                 "(iterators)\n";

    std::cout << "[std_deque] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/std_deque", run>;

}  // namespace
