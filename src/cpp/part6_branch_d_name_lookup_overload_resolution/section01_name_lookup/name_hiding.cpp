// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 名字隐藏)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : name_hiding
// Topic id : part6/d/section01/name_hiding
//
// 要点: 派生类同名成员隐藏基类【所有】同名重载；using 重新引入。
// 参考: [class.member.lookup]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

struct Base {
    std::string f(int) { return "Base::f(int)"; }
    std::string f(double) { return "Base::f(double)"; }
    std::string g() { return "Base::g"; }
};

struct DerivedHidden : Base {
    std::string f(double) { return "Derived::f(double)"; }
    // Base::f(int) 被隐藏
};

struct DerivedUsing : Base {
    using Base::f;
    std::string f(double) { return "DerivedUsing::f(double)"; }
};

// 内层作用域隐藏外层
[[maybe_unused]] int value = 1;

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif
int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 name hiding ===\n";

    [[maybe_unused]] DerivedHidden dh;
    assert(dh.f(5) == "Derived::f(double)");  // 5→double，不是 Base::f(int)
    assert(dh.f(1.5) == "Derived::f(double)");
    assert(dh.Base::f(5) == "Base::f(int)");
    assert(dh.g() == "Base::g");  // 未隐藏

    [[maybe_unused]] DerivedUsing du;
    assert(du.f(5) == "Base::f(int)");
    assert(du.f(1.5) == "DerivedUsing::f(double)");

    {
        [[maybe_unused]] int value = 99;
        assert(value == 99);
        assert(::value == 1);
    }

    std::cout << "  derived name hides ALL base overloads of that name\n";
    std::cout << "  fix: using Base::f; or call Base::f explicitly\n";
    std::cout << "name_hiding: OK\n";
    return 0;
}

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/name_hiding", run>;

}  // namespace
