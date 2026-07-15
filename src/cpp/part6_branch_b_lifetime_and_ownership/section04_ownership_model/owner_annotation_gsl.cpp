// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B7 gsl::owner / B12 验收)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : owner_annotation_gsl
// Topic id : part6/b/section04/owner_annotation_gsl
//
// 要点: owner<T*> 静态标注「我负责释放」；终极形态 unique_ptr。
// 验收: 用 owner/not_null/span 把裸指针 API 改成所有权显式。
// 参考: microsoft/GSL  C++ CG R.3

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

namespace {

// GSL owner 本质：类型别名，供静态分析/文档；无运行时开销
template <class T>
using owner = T;

struct Widget {
    int id;
    explicit Widget(int i) : id(i) {}
};

// --- 旧风格：返回 owner 裸指针（调用方 delete）---
owner<Widget*> create_raw() {
    return new Widget{1};
}
void destroy_raw(owner<Widget*> p) {
    delete p;
}

// --- 更好：unique_ptr 强制所有权 ---
std::unique_ptr<Widget> create_unique() {
    return std::make_unique<Widget>(2);
}

// --- 进阶: 改造前/后 API 对照 ---
// Before: Widget* factory(); void sink(Widget*);  // 谁 delete？
// After:  owner<Widget*> factory(); void sink(owner<Widget*>);
// Best:   unique_ptr<Widget> factory(); void sink(unique_ptr<Widget>);

struct LegacyApi {
    // 文档：返回值由调用方拥有
    static owner<Widget*> open(int id) { return new Widget{id}; }
    static void close(owner<Widget*> p) { delete p; }
};

struct ModernApi {
    static std::unique_ptr<Widget> open(int id) { return std::make_unique<Widget>(id); }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B7 owner annotation (GSL-style) ===\n";

    // 入门
    owner<Widget*> p = create_raw();
    assert(p && p->id == 1);
    destroy_raw(p);
    p = nullptr;

    auto u = create_unique();
    assert(u && u->id == 2);

    // 进阶: Legacy vs Modern
    owner<Widget*> leg = LegacyApi::open(7);
    assert(leg->id == 7);
    LegacyApi::close(leg);

    auto mod = ModernApi::open(8);
    assert(mod->id == 8);
    // 离开作用域自动释放；无法「忘记 delete」

    // 专家: owner<> 不阻止误用（仍可漏 delete）；它是给人类 + 静态分析的标注
    // 类型系统强制 → unique_ptr / shared_ptr
    std::cout << "  owner<> documents ownership for analysis tools\n";
    std::cout << "  unique_ptr enforces ownership in the type system\n";
    std::cout << "  B12: modernize raw owning pointers with owner or unique_ptr\n";
    std::cout << "owner_annotation_gsl: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/owner_annotation_gsl", run>;

}  // namespace
