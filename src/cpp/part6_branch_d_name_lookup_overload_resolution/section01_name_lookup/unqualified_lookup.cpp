// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 unqualified)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : unqualified_lookup
// Topic id : part6/d/section01/unqualified_lookup
//
// 要点: 无 :: 的名字从内层作用域向外找，命中即停。
// 参考: [basic.lookup.unqual]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

int x = 1;

namespace ns {
int x = 2;
int probe() {
    int x = 3;
    return x;  // 局部
}
int outer() {
    return x;  // ns::x
}
}  // namespace ns

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 unqualified lookup ===\n";

    assert(x == 1);
    assert(ns::probe() == 3);
    assert(ns::outer() == 2);

    {
        int x = 99;
        assert(x == 99);
    }
    assert(::x == 1);

    std::cout << "unqualified_lookup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/unqualified_lookup", run>;

}  // namespace
