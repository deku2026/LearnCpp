// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B7 gsl::owner)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : owner_annotation_gsl
// Topic id : part6/b/section04/owner_annotation_gsl
//
// 要点: owner<T*> 是静态标注「我负责释放」；终极形态仍是 unique_ptr。
//       不依赖 microsoft/GSL 包——本地等价 typedef + 语义演示。
// 参考: https://github.com/microsoft/GSL  C++ CG R.3

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

namespace {

// GSL owner 本质：类型别名，供静态分析/文档；无运行时开销
template <class T>
using owner = T;  // gsl::owner<T*> 即 owner 标注的指针类型

struct Widget {
    int id;
    explicit Widget(int i) : id(i) {}
};

// 旧风格：返回 owner 裸指针（调用方负责 delete）
owner<Widget*> create_raw() {
    return new Widget{1};
}

void destroy_raw(owner<Widget*> p) {
    delete p;
}

// 更好：unique_ptr 即所有权 + 自动释放
std::unique_ptr<Widget> create_unique() {
    return std::make_unique<Widget>(2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B7 owner annotation (GSL-style) ===\n";

    owner<Widget*> p = create_raw();
    assert(p && p->id == 1);
    destroy_raw(p);
    p = nullptr;

    auto u = create_unique();
    assert(u && u->id == 2);
    // 离开作用域自动 delete

    // API 对照：
    // owner<T*> create();     // 文档：调用方拥有
    // unique_ptr<T> create(); // 类型系统强制所有权

    std::cout << "  owner<> documents ownership; unique_ptr enforces it\n";
    std::cout << "owner_annotation_gsl: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/owner_annotation_gsl", run>;

}  // namespace
