// Topic    : unique_ptr —— 独占所有权、不可拷贝、可移动
// Doc      : 第2部分-阶段5 · 步骤 9.1–9.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : unique_ptr_exclusive
// Topic id : part2/stage05/section04/unique_ptr_exclusive
// Refs     : https://en.cppreference.com/w/cpp/memory/unique_ptr
//            Effective Modern C++ Item 18

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace {

struct Widget {
    int id;
    explicit Widget(int i) : id(i) { std::cout << "  +Widget " << id << '\n'; }
    ~Widget() { std::cout << "  -Widget " << id << '\n'; }
};

void take_ownership(std::unique_ptr<Widget> p) {
    assert(p);
    std::cout << "  take_ownership id=" << p->id << '\n';
}  // p 析构 → delete Widget

void observe(const Widget* p) {
    assert(p);
    std::cout << "  observe id=" << p->id << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [unique_ptr_exclusive] 入门：独占 + 自动释放 ===\n";

    {
        auto w = std::make_unique<Widget>(1);
        assert(w && w->id == 1);
        std::cout << "id=" << w->id << '\n';
    }  // 自动析构 Widget 1

    std::cout << "=== 进阶：移动转移；不可拷贝 ===\n";
    {
        auto a = std::make_unique<Widget>(2);
        // auto b = a;  // 错误：unique_ptr 不可拷贝
        auto b = std::move(a);
        assert(b && b->id == 2);
        assert(a == nullptr);  // moved-from 保证为空

        observe(b.get());              // 借用：不转移所有权
        take_ownership(std::move(b));  // 显式交出所有权
        assert(b == nullptr);
    }

    std::cout << "=== 进阶：容器中的 unique_ptr ===\n";
    {
        std::vector<std::unique_ptr<Widget>> vec;
        vec.push_back(std::make_unique<Widget>(10));
        vec.push_back(std::make_unique<Widget>(11));
        auto extra = std::make_unique<Widget>(12);
        vec.push_back(std::move(extra));
        assert(extra == nullptr);
        assert(vec.size() == 3);
        assert(vec[0]->id == 10);
        std::cout << "vector holds " << vec.size() << " exclusive Widgets\n";
    }

    std::cout << "=== 专家：零开销与接口约定 ===\n";
    // · 默认删除器时 sizeof(unique_ptr<T>) == sizeof(T*)
    // · 函数要拿走所有权：按值收 unique_ptr
    // · 只借用：T* / T& / const T*
    // · 派生 → 基类：Base 需虚析构（见文档 9.4）
    static_assert(sizeof(std::unique_ptr<int>) == sizeof(int*));

    struct Base {
        virtual ~Base() = default;
        virtual int who() const { return 0; }
    };
    struct Derived : Base {
        int who() const override { return 1; }
    };
    std::unique_ptr<Base> pb = std::make_unique<Derived>();
    assert(pb->who() == 1);

    std::cout << "=== unique_ptr_exclusive: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/unique_ptr_exclusive", run>;

}  // namespace
