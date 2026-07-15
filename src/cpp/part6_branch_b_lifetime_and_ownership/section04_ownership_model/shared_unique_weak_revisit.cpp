// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B5 所有权模型 / B11 清单)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : shared_unique_weak_revisit
// Topic id : part6/b/section04/shared_unique_weak_revisit
//
// 要点: unique 独占 / shared 共享 / weak 不拥有；循环用 weak 打破。
// 陷阱: shared_ptr 环泄漏；shared_from_this 误用双重释放。
// 参考: [smartptr]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

namespace {

struct Node : std::enable_shared_from_this<Node> {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 打破环
    explicit Node(std::string n) : name(std::move(n)) {}
};

// ❌ 危险: return shared_ptr<Node>(this); // 若已有 shared 拥有者 → 双重释放
// ✅ shared_from_this() 要求对象已由 shared_ptr 管理

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B5 unique / shared / weak revisit ===\n";

    // --- unique ---
    {
        auto u = std::make_unique<int>(42);
        assert(u && *u == 42);
        auto v = std::move(u);
        assert(!u && v && *v == 42);
    }

    // --- shared 引用计数 ---
    {
        auto a = std::make_shared<std::string>("hi");
        assert(a.use_count() == 1);
        auto b = a;
        assert(a.use_count() == 2);
        assert(*b == "hi");
        b.reset();
        assert(a.use_count() == 1);
    }

    // --- weak ---
    {
        std::weak_ptr<int> w;
        {
            auto s = std::make_shared<int>(7);
            w = s;
            assert(w.use_count() == 1);
            assert(!w.expired());
            auto locked = w.lock();
            assert(locked && *locked == 7);
            assert(w.use_count() == 2);
        }
        assert(w.expired());
    }

    // --- 环: child.prev 用 weak ---
    {
        auto p = std::make_shared<Node>("parent");
        auto c = std::make_shared<Node>("child");
        p->next = c;
        c->prev = p;
        assert(p.use_count() == 1);
        assert(c.use_count() == 2);  // p->next 与 c
        auto self = p->shared_from_this();
        assert(self.use_count() == 2);
        self.reset();
        p.reset();
        assert(c->prev.expired());
        c.reset();
    }

    // 进阶: 若 next/prev 都用 shared → 环，use_count 永不归零（泄漏）
    // 专家: 优先 unique_ptr；shared 仅当共享所有权是真实需求

    std::cout << "  prefer unique_ptr; shared only when needed; weak for cycles\n";
    std::cout << "  enable_shared_from_this: never shared_ptr(this) ad-hoc\n";
    std::cout << "shared_unique_weak_revisit: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/shared_unique_weak_revisit", run>;

}  // namespace
