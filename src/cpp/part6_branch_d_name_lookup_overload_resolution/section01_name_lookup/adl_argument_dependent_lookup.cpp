// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D2 ADL)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_argument_dependent_lookup
// Topic id : part6/d/section01/adl_argument_dependent_lookup
//
// 要点: 非限定函数调用还在实参关联命名空间查找。
// 参考: [basic.lookup.argdep]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

namespace lib {
struct Widget {
    int v;
};
std::string describe(const Widget& w) {
    return "Widget(" + std::to_string(w.v) + ")";
}
int tag(const Widget&) {
    return 1;
}
}  // namespace lib

namespace other {
int tag(int) {
    return 2;
}
}  // namespace other

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D2 ADL (argument-dependent lookup) ===\n";

    lib::Widget w{42};
    // 非限定 describe(w)：ADL 在 lib 找到
    assert(describe(w) == "Widget(42)");

    // 实参 Widget → 关联 lib
    assert(tag(w) == 1);

    // 仅 int 时不会找到 lib::tag
    using other::tag;
    assert(tag(0) == 2);

    std::cout << "adl_argument_dependent_lookup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/adl_argument_dependent_lookup", run>;

}  // namespace
