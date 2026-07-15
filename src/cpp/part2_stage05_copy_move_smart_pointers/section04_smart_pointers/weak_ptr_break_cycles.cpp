// Topic    : weak_ptr 打破 shared_ptr 循环引用
// Doc      : 第2部分-阶段5 · 步骤 10.3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : weak_ptr_break_cycles
// Topic id : part2/stage05/section04/weak_ptr_break_cycles
// Refs     : https://en.cppreference.com/w/cpp/memory/weak_ptr
//            Effective Modern C++ Item 20

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace {

int g_node_dtors = 0;

// 错误：双向 shared_ptr → 循环泄漏
struct LeakyNode {
    std::string name;
    std::shared_ptr<LeakyNode> next;
    explicit LeakyNode(std::string n) : name(std::move(n)) {}
    ~LeakyNode() {
        ++g_node_dtors;
        std::cout << "  ~LeakyNode " << name << '\n';
    }
};

// 正确：一个方向 weak_ptr
struct Node {
    std::string name;
    std::shared_ptr<Node> next;  // 拥有
    std::weak_ptr<Node> prev;    // 观察，不增加强计数
    explicit Node(std::string n) : name(std::move(n)) {}
    ~Node() {
        ++g_node_dtors;
        std::cout << "  ~Node " << name << '\n';
    }
};

// 说明：若 a->next=b 且 b->next=a 后直接离开作用域，强计数永不归零 → 泄漏。
// 下面用「可拆环」版本演示问题并保证进程不残留泄漏。
void demo_leak_then_manual_break_for_process_hygiene() {
    // 可观察泄漏：先建环，再手动拆，避免测试进程 RSS 涨
    g_node_dtors = 0;
    auto a = std::make_shared<LeakyNode>("A2");
    auto b = std::make_shared<LeakyNode>("B2");
    a->next = b;
    b->next = a;
    assert(a.use_count() == 2);
    assert(b.use_count() == 2);
    // 拆环后才能释放
    b->next.reset();
    a->next.reset();
    a.reset();
    b.reset();
    assert(g_node_dtors == 2);
    std::cout << "after manual break: dtors=" << g_node_dtors << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [weak_ptr_break_cycles] 入门：循环问题 ===\n";
    demo_leak_then_manual_break_for_process_hygiene();

    std::cout << "=== 进阶：weak_ptr 解法 ===\n";
    g_node_dtors = 0;
    {
        auto a = std::make_shared<Node>("A");
        auto b = std::make_shared<Node>("B");
        a->next = b;  // A 强持有 B
        b->prev = a;  // B 弱观察 A
        assert(a.use_count() == 1);
        assert(b.use_count() == 2);  // a->next 与 b

        if (auto sp = b->prev.lock()) {
            assert(sp->name == "A");
            std::cout << "prev.lock() -> " << sp->name << '\n';
        } else {
            assert(false);
        }
    }  // 全部释放，应两次析构
    assert(g_node_dtors == 2);
    std::cout << "no cycle: dtors=" << g_node_dtors << '\n';

    std::cout << "=== 进阶：lock / expired ===\n";
    {
        std::weak_ptr<Node> w;
        {
            auto n = std::make_shared<Node>("temp");
            w = n;
            assert(!w.expired());
            assert(w.use_count() == 1);
        }
        assert(w.expired());
        auto sp = w.lock();
        assert(!sp);
        std::cout << "expired weak -> lock() empty\n";
    }

    std::cout << "=== 专家：树 / 图的所有权约定 ===\n";
    // · 父拥有子：parent → child 用 unique_ptr 或 shared_ptr
    // · 子指父：child → parent 用 raw T* 或 weak_ptr
    // · 图中至少拆断环上的一条强边
    // · weak_ptr 不能 * / ->，必须 lock 成 shared_ptr 再访问

    std::cout << "=== weak_ptr_break_cycles: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/weak_ptr_break_cycles", run>;

}  // namespace
