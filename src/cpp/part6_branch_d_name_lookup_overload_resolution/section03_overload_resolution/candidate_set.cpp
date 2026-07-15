// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D5 候选集)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : candidate_set
// Topic id : part6/d/section03/candidate_set
//
// 要点: 候选 = 普通查找 + ADL 找到的同名函数（含模板、成员）。
// 参考: [over.match]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

namespace alpha {
struct A {};
std::string f(A) {
    return "alpha::f(A)";
}
}  // namespace alpha

namespace beta {
std::string f(int) {
    return "beta::f(int)";
}
}  // namespace beta

using beta::f;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D5 candidate set ===\n";

    // 候选含 using 引入的 beta::f 与 ADL 的 alpha::f
    assert(f(0) == "beta::f(int)");
    assert((f(alpha::A{}) == "alpha::f(A)"));

    std::cout << "  candidates from ordinary lookup + ADL\n";
    std::cout << "candidate_set: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/candidate_set", run>;

}  // namespace
