// Topic    : 裸指针 = 借用/观察（不拥有）；所有权用智能指针表达
// Doc      : 第2部分-阶段5 · 步骤 8
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : raw_pointer_as_borrow
// Topic id : part2/stage05/section04/raw_pointer_as_borrow
// Refs     : C++ Core Guidelines R.20 / R.21 / R.30 / F.7
//            Effective Modern C++ Item 18–22

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

// 约定：
// · unique_ptr / shared_ptr 参数 → 涉及所有权
// · T* / T& / const T&            → 借用，不释放

void observe(const int* p) {
    // 可空观察
    if (p) {
        std::cout << "  observe *p=" << *p << '\n';
    } else {
        std::cout << "  observe null\n";
    }
}

void observe_ref(const int& r) {
    // 不可空
    std::cout << "  observe_ref " << r << '\n';
}

void take_unique(std::unique_ptr<int> p) {
    assert(p);
    std::cout << "  take_unique " << *p << '\n';
}  // 释放

void share(std::shared_ptr<std::string> p) {
    std::cout << "  share use_count=" << p.use_count() << " value=" << *p << '\n';
}

// 返回借用：调用方不得 delete；生命周期由调用方/拥有者保证
const std::string* find_name(const std::vector<std::string>& names, std::size_t i) {
    if (i >= names.size()) {
        return nullptr;
    }
    return &names[i];
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [raw_pointer_as_borrow] 入门：所有权 vs 借用 ===\n";

    auto owner = std::make_unique<int>(42);
    observe(owner.get());  // 借用
    observe_ref(*owner);   // 借用（非空）
    assert(owner && *owner == 42);

    // take_unique(owner);     // 错误：不能拷贝
    take_unique(std::move(owner));  // 显式转移
    assert(!owner);

    std::cout << "=== 进阶：shared 与可空观察 ===\n";
    {
        auto s = std::make_shared<std::string>("data");
        share(s);  // 共享所有权副本
        assert(s.use_count() == 1);

        const int* p = nullptr;
        observe(p);
    }

    std::cout << "=== 进阶：API 设计口诀 ===\n";
    // 参数：
    // · 拿走独占所有权     → unique_ptr<T> 按值
    // · 共享所有权         → shared_ptr<T> 按值
    // · 只读不拥有且可空   → const T*
    // · 只读不拥有且非空   → const T&
    // · 可改不拥有且非空   → T&
    // 返回：
    // · 交出新所有权       → unique_ptr / shared_ptr / 值
    // · 返回内部观察       → T* / T&（文档写清生命周期）

    std::vector<std::string> names{"alice", "bob"};
    const std::string* n = find_name(names, 1);
    assert(n && *n == "bob");
    assert(find_name(names, 9) == nullptr);
    std::cout << "find_name borrow -> " << *n << '\n';

    std::cout << "=== 专家：Qt 对照（勿混） ===\n";
    // Qt 常用裸指针 + QObject 父子树表示所有权
    // 现代 C++ 非 QObject 资源：智能指针拥有，裸指针仅借用
    // 两套体系不要混用同一对象的生命周期管理

    std::cout << "=== raw_pointer_as_borrow: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/raw_pointer_as_borrow", run>;

}  // namespace
