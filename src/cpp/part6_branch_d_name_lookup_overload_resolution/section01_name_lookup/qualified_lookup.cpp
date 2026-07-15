// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 qualified)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : qualified_lookup
// Topic id : part6/d/section01/qualified_lookup
//
// 要点: A::b / ::b 只在指定作用域查找。
// 参考: [basic.lookup.qual]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

namespace lib {
struct Widget {
    static constexpr int id = 7;
    int value = 0;
};
int tag = 42;
}  // namespace lib

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 qualified lookup ===\n";

    assert(lib::tag == 42);
    assert(lib::Widget::id == 7);

    lib::Widget w;
    w.value = 3;
    assert(w.value == 3);

    // 全局限定
    assert(::lib::tag == 42);

    // 标准库限定名
    std::vector<int> v{1, 2};
    assert(v.size() == 2);

    std::cout << "qualified_lookup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/qualified_lookup", run>;

}  // namespace
