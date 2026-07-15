// LearnCpp topic
// Doc      : 第6部分-支线G · 生命周期违规
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : lifetime_violation_deep
// Topic id : part6/g/section03/lifetime_violation_deep
//
// 要点: 对象生命周期外访问 = UB; 注意悬空引用、placement new、const 对象。
// 不触发 UB。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <new>
#include <string>

namespace {

struct Widget {
    int id;
    explicit Widget(int i) : id(i) {}
    ~Widget() { id = -1; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G lifetime violation (safe patterns) ===\n";

    // 正确 placement new 生命周期
    alignas(Widget) unsigned char buf[sizeof(Widget)];
    Widget* w = new (buf) Widget(3);
    assert(w->id == 3);
    w->~Widget();  // 显式结束生命周期

    // 智能指针管理
    {
        auto p = std::make_unique<Widget>(9);
        assert(p->id == 9);
    }

    // 绑定临时: 完整表达式内合法
    const std::string& r = std::string("tmp");
    assert(r == "tmp");  // 临时绑定到 const 引用, 生命延长到 r 作用域

    // 危险: 返回局部引用 / 使用已析构对象 — 不演示

    std::cout << "  after destroy, do not use pointers unless launder/start_lifetime_as rules apply\n";
    std::cout << "lifetime_violation_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/lifetime_violation_deep", run>;

}  // namespace
