// Topic    : make_shared 一次分配优化 vs shared_ptr(new T)
// Doc      : 第2部分-阶段5 · 步骤 10.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_shared_single_allocation
// Topic id : part2/stage05/section04/make_shared_single_allocation
// Refs     : https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared
//            Effective Modern C++ Item 21

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace {

struct Big {
    char payload[256];
    int tag;
    explicit Big(int t) : tag(t) {
        payload[0] = 'B';
        std::cout << "  +Big tag=" << tag << '\n';
    }
    ~Big() { std::cout << "  -Big tag=" << tag << '\n'; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [make_shared_single_allocation] 入门：对比 ===\n";
    // shared_ptr<T>(new T(...))：
    //   至少两次堆分配 —— 对象一次，控制块一次
    // make_shared<T>(...)：
    //   通常一次分配 —— 对象与控制块同一块内存
    // 好处：更快、碎片更少、异常安全、缓存局部性更好

    auto p1 = std::shared_ptr<int>(new int(42));  // 两次分配路径
    auto p2 = std::make_shared<int>(42);          // 一次分配路径
    assert(*p1 == 42 && *p2 == 42);
    std::cout << "both hold 42; prefer make_shared in general\n";

    std::cout << "=== 进阶：验收点 —— 好在哪 ===\n";
    // 1) 少一次堆分配
    // 2) 不裸写 new：异常安全（避免 new 成功但 shared_ptr 构造前失败的窗口）
    // 3) 对象与控制块相邻 → 更好的局部性

    auto b = std::make_shared<Big>(7);
    assert(b->tag == 7);
    assert(b.use_count() == 1);
    {
        auto c = b;
        assert(b.use_count() == 2);
    }
    assert(b.use_count() == 1);

    std::cout << "=== 进阶：make_shared 的代价（大对象 + 长期 weak） ===\n";
    {
        std::weak_ptr<Big> w;
        {
            auto sp = std::make_shared<Big>(99);
            w = sp;
            assert(!w.expired());
        }  // 强计数 0 → Big 析构执行
        // 但 make_shared 下控制块与对象同块：只要 weak 仍在，整块内存（含 sizeof(Big)）
        // 不能还给堆 —— 大对象 + 长期 weak 时可能浪费
        assert(w.expired());
        std::cout << "after strong gone: weak expired; memory may linger until weak dies\n";
    }  // weak 销毁 → 控制块+对象存储一并释放

    std::cout << "=== 专家：何时用 shared_ptr(new T) ===\n";
    // · 需要自定义分配器/删除器且 make_shared 不便时
    // · 大对象 + 可能长期 weak 观察，希望对象内存尽早释放
    // · 一般代码路径：默认 make_shared / allocate_shared

    auto with_del = std::shared_ptr<std::string>(new std::string("x"), [](std::string* p) {
        std::cout << "  custom delete string\n";
        delete p;
    });
    assert(*with_del == "x");

    std::cout << "=== make_shared_single_allocation: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/make_shared_single_allocation", run>;

}  // namespace
