// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part2/stage07/section05/constexpr_unique_ptr_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/memory/unique_ptr
//   P2273R3 — Making std::unique_ptr constexpr
//   路线图验收：constexpr vector + unique_ptr 编译期处理数据

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — C++23：unique_ptr / make_unique 可在常量求值中使用
// ---------------------------------------------------------------------------

constexpr int simple_unique_value() {
    auto p = std::make_unique<int>(42);
    return *p;  // p 在求值结束前销毁 → 瞬态分配 OK
}

// ---------------------------------------------------------------------------
// §进阶 — unique_ptr 拥有 vector / 数组；转移所有权
// ---------------------------------------------------------------------------

constexpr int compute_with_smart_ptr() {
    auto ptr = std::make_unique<std::vector<int>>();
    for (int i = 1; i <= 5; ++i) {
        ptr->push_back(i);
    }
    return std::accumulate(ptr->begin(), ptr->end(), 0);  // 15
}

constexpr int unique_array_sum() {
    auto p = std::make_unique<int[]>(4);
    p[0] = 1;
    p[1] = 2;
    p[2] = 3;
    p[3] = 4;
    int s = 0;
    for (int i = 0; i < 4; ++i) {
        s += p[i];
    }
    return s;
}

constexpr int transfer_ownership_demo() {
    auto a = std::make_unique<int>(7);
    std::unique_ptr<int> b = std::move(a);
    // a 为空，b 拥有 7
    if (!b) {
        return -1;
    }
    return *b;
}

// ---------------------------------------------------------------------------
// §专家 — 为何需要；与裸 new 对照；自定义 deleter 边界
// ---------------------------------------------------------------------------
// 1) constexpr 动态结构过去只能 new/delete；unique_ptr 带来异常安全心智
//    与 RAII，在编译期同样成立（析构必须在常量求值内完成）。
// 2) 默认 deleter 的 delete 表达式在 C++20 分配模型下可 constexpr。
// 3) 有状态/函数指针 deleter 的可 constexpr 程度依赖实现与类型是否字面值。
// 4) shared_ptr 的 constexpr 支持更晚/更窄——不要与 unique_ptr 混谈。

struct Node {
    int value;
    std::unique_ptr<Node> next;
    constexpr explicit Node(int v) : value(v), next(nullptr) {}
};

constexpr int list_sum() {
    auto head = std::make_unique<Node>(1);
    head->next = std::make_unique<Node>(2);
    head->next->next = std::make_unique<Node>(3);
    int s = 0;
    for (Node* cur = head.get(); cur != nullptr; cur = cur->next.get()) {
        s += cur->value;
    }
    return s;  // 6
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/constexpr_unique_ptr_cpp23 ===\n";

    static_assert(simple_unique_value() == 42);
    static_assert(compute_with_smart_ptr() == 15);
    static_assert(unique_array_sum() == 10);
    static_assert(transfer_ownership_demo() == 7);
    static_assert(list_sum() == 6);

    std::cout << "[intro] make_unique<int>(42) -> " << simple_unique_value() << '\n';
    std::cout << "[advanced] vector via unique_ptr sum=" << compute_with_smart_ptr()
              << " array sum=" << unique_array_sum() << " move demo=" << transfer_ownership_demo() << '\n';
    std::cout << "[expert] constexpr intrusive list sum=" << list_sum()
              << " (unique_ptr links, all destroyed before CTCE ends)\n";

    // 运行期同样代码
    auto p = std::make_unique<std::vector<int>>();
    p->push_back(1);
    p->push_back(2);
    assert(std::accumulate(p->begin(), p->end(), 0) == 3);

    std::cout << "constexpr_unique_ptr_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_unique_ptr_cpp23", run>;

}  // namespace
