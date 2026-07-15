// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D5 候选集)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : candidate_set
// Topic id : part6/d/section03/candidate_set
//
// 要点: 候选 = 普通查找 + ADL 找到的同名函数、函数模板、成员候选。
// 参考: [over.match]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

namespace alpha {
struct A {};
[[maybe_unused]] std::string f(A) {
    return "alpha::f(A)";
}
}  // namespace alpha

namespace beta {
[[maybe_unused]] std::string f(int) {
    return "beta::f(int)";
}
[[maybe_unused]] std::string f(double) {
    return "beta::f(double)";
}
}  // namespace beta

using beta::f;  // 把 beta 的 f 引入当前作用域（普通查找候选）

struct S {
    std::string call(int) { return "S::call(int)"; }
};

[[maybe_unused]] std::string call(S&, double) {
    return "free call(S&,double)";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D5 candidate set ===\n";

    // 普通查找: using 引入的 beta::f
    assert(f(1) == "beta::f(int)");
    assert(f(1.5) == "beta::f(double)");

    // ADL 加入 alpha::f
    [[maybe_unused]] alpha::A a;
    assert(f(a) == "alpha::f(A)");

    // 成员 vs 自由函数: 成员调用语法 .call 走成员候选
    [[maybe_unused]] S s;
    assert(s.call(1) == "S::call(int)");
    assert(call(s, 2.0) == "free call(S&,double)");

    std::cout << "  candidates from: ordinary lookup + ADL + member rules\n";
    std::cout << "  next steps: viable filter → best viable (ICS rank)\n";
    std::cout << "candidate_set: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/candidate_set", run>;

}  // namespace
