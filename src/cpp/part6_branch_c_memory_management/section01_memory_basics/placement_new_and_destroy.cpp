// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C3 placement new)
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : placement_new_and_destroy
// Topic id : part6/c/section01/placement_new_and_destroy
//
// 要点: placement new 在已有内存构造；必须显式析构；勿 delete 非 new 内存。
// 参考: [expr.new] construct_at/destroy_at

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <string>

namespace {

struct Widget {
    std::string name;
    int id;
    Widget(std::string n, int i) : name(std::move(n)), id(i) {}
    ~Widget() { id = -1; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C3 placement new + explicit destroy ===\n";

    alignas(Widget) std::byte buffer[sizeof(Widget)];

    Widget* p = std::construct_at(reinterpret_cast<Widget*>(buffer), "w", 42);
    assert(p->id == 42);
    assert(p->name == "w");

    std::destroy_at(p);
    // delete p; // ❌ buffer 不是 operator new 来的

    // 复用同一缓冲
    p = std::construct_at(reinterpret_cast<Widget*>(buffer), "reuse", 7);
    assert(p->name == "reuse");
    std::destroy_at(p);

    // 经典 placement new 语法
    alignas(int) std::byte ibuf[sizeof(int)];
    int* ip = new (ibuf) int{99};
    assert(*ip == 99);
    // trivial 可省略析构
    (void)ip;

    std::cout << "  new T: alloc+ctor+lifetime; placement: ctor+lifetime only\n";
    std::cout << "placement_new_and_destroy: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/placement_new_and_destroy", run>;

}  // namespace
