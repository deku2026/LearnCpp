// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D4 两阶段查找)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : two_phase_lookup
// Topic id : part6/d/section02/two_phase_lookup
//
// 要点: 非依赖名定义期查找；依赖名实例化期查找（含 ADL）。
// 经典「GCC 过 Clang 不过」多与此相关。
// 参考: [temp.res] [temp.dep]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

namespace client {
struct Item {
    int n;
};
[[maybe_unused]] std::string g(const Item& it) {
    return "Item:" + std::to_string(it.n);
}
}  // namespace client

std::string helper() {
    return "helper";
}

template <typename T>
std::string f(T t) {
    auto a = helper();  // 非依赖 → 阶段1（定义点）
    auto b = g(t);      // 依赖调用 → 阶段2 + ADL
    return a + "|" + b;
}

template <typename T>
struct Base {
    int value = 11;
};

template <typename T>
struct Derived : Base<T> {
    int get() const {
        // return value; // 依赖基类成员：未限定可能找不到
        return this->value;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D4 two-phase lookup ===\n";

    assert((f(client::Item{7}) == "helper|Item:7"));

    [[maybe_unused]] Derived<int> d;
    assert(d.get() == 11);

    std::cout << "  phase1: non-dependent names at template definition\n";
    std::cout << "  phase2: dependent names at instantiation (+ ADL)\n";
    std::cout << "  dependent base members: use this-> or Base<T>::\n";
    std::cout << "two_phase_lookup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section02/two_phase_lookup", run>;

}  // namespace
