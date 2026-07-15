// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 名字隐藏)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : name_hiding
// Topic id : part6/d/section01/name_hiding
//
// 要点: 派生类同名成员隐藏基类【所有】同名重载；using 引入解除隐藏。
// 参考: [class.member.lookup]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

struct Base {
    std::string f(int) { return "Base::f(int)"; }
    std::string f(double) { return "Base::f(double)"; }
};

struct DerivedHidden : Base {
    std::string f(double) { return "Derived::f(double)"; }
    // Base::f(int) 被隐藏
};

struct DerivedUsing : Base {
    using Base::f;
    std::string f(double) { return "DerivedUsing::f(double)"; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 name hiding ===\n";

    DerivedHidden dh;
    // dh.f(5) 会调 f(double)（5→double），不是 Base::f(int)
    assert(dh.f(5) == "Derived::f(double)");
    assert(dh.f(1.5) == "Derived::f(double)");
    // 仍可限定调用基类
    assert(dh.Base::f(5) == "Base::f(int)");

    DerivedUsing du;
    assert(du.f(5) == "Base::f(int)");  // using 后 int 重载可见
    assert(du.f(1.5) == "DerivedUsing::f(double)");

    std::cout << "name_hiding: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/name_hiding", run>;

}  // namespace
