// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C5 launder)
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : std_launder_when_needed
// Topic id : part6/c/section04/std_launder_when_needed
//
// 要点: launder 不创建对象；对象已在原地替换时洗白指针（const 成员场景）。
// 参考: https://en.cppreference.com/w/cpp/utility/launder

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <new>

namespace {

struct X {
    const int n;
    explicit X(int v) : n(v) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C5 std::launder when needed ===\n";

    alignas(X) unsigned char buf[sizeof(X)];
    X* p1 = new (buf) X{1};
    assert(p1->n == 1);
    p1->~X();

    X* p2 = new (buf) X{2};
    assert(p2->n == 2);

    // 通过可能被优化的旧指针路径：用 launder 获得当前对象
    int v = std::launder(reinterpret_cast<X*>(buf))->n;
    assert(v == 2);
    assert(std::launder(p1)->n == 2);

    p2->~X();

    // 分工: placement new=构造; start_lifetime_as=仅起生命; launder=修指针
    std::cout << "  launder requires an object already alive at the address\n";
    std::cout << "std_launder_when_needed: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/std_launder_when_needed", run>;

}  // namespace
