// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D2 ADL)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_argument_dependent_lookup
// Topic id : part6/d/section01/adl_argument_dependent_lookup
//
// 要点: 非限定函数调用还会在实参关联命名空间里找。
// 这是 cout<<x / swap / begin 能工作的根因。
// 参考: [basic.lookup.argdep]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>

namespace {

namespace lib {
struct Widget {
    int v;
};
[[maybe_unused]] std::string describe(const Widget& w) {
    return "Widget(" + std::to_string(w.v) + ")";
}
[[maybe_unused]] int tag(const Widget&) {
    return 1;
}
void touch(Widget& w) {
    ++w.v;
}
}  // namespace lib

namespace other {
[[maybe_unused]] int tag(int) {
    return 2;
}
}  // namespace other

// 进阶: 关联集含基类/模板实参的命名空间
namespace base_ns {
struct Base {};
void mark(const Base&) {}
}  // namespace base_ns

namespace der_ns {
struct Der : base_ns::Base {};
// mark 可通过 Base 关联到 base_ns
}  // namespace der_ns

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D2 ADL (argument-dependent lookup) ===\n";

    lib::Widget w{42};
    assert(describe(w) == "Widget(42)");  // 非限定 + ADL → lib
    assert(tag(w) == 1);

    using other::tag;
    assert(tag(0) == 2);

    touch(w);
    assert(w.v == 43);

    der_ns::Der d;
    mark(d);  // ADL via base class namespace

    // 专家: 限定调用 lib::describe(w) 不做 ADL 扩展候选集规则不同
    assert(lib::describe(w) == "Widget(43)");
    // 函数指针赋值通常不做 ADL: auto *fp = describe; // 需可见声明

    std::cout << "  ADL associated namespaces: args' classes/enums/templates\n";
    std::cout << "  enables operator<<, swap, begin without using-directives\n";
    std::cout << "adl_argument_dependent_lookup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/adl_argument_dependent_lookup", run>;

}  // namespace
