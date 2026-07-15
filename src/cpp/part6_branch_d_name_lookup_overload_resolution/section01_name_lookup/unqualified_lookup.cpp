// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 unqualified)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : unqualified_lookup
// Topic id : part6/d/section01/unqualified_lookup
//
// 要点: 无 :: 的名字从内层作用域向外找，找到即停。
// 参考: [basic.lookup.unqual]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

int x = 1;

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif
namespace ns {
int x = 2;
[[maybe_unused]] int probe() {
    int x = 3;
    return x;  // 局部
}
[[maybe_unused]] int outer() {
    return x;
}  // ns::x
[[maybe_unused]] void show() {
    // 类作用域示例见 name_hiding / injected_class_name
}
}  // namespace ns

struct S {
    int x = 10;
    int get() const { return x; }  // 成员
    int get_global() const { return ::x; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 unqualified lookup ===\n";

    assert(x == 1);
    assert(ns::probe() == 3);
    assert(ns::outer() == 2);

    {
        [[maybe_unused]] int x = 99;
        assert(x == 99);
    }
    assert(::x == 1);

    [[maybe_unused]] S s;
    assert(s.get() == 10);
    assert(s.get_global() == 1);

    std::cout << "  order: block → function → class → namespace → global\n";
    std::cout << "  stops at first match (no merge of outer overloads)\n";
    std::cout << "unqualified_lookup: OK\n";
    return 0;
}

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/unqualified_lookup", run>;

}  // namespace
