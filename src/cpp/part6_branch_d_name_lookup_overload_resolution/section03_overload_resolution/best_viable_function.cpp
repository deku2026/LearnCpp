// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D5 best viable)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : best_viable_function
// Topic id : part6/d/section03/best_viable_function
//
// 要点: 严格更优转换序列者胜；同级则歧义。
// 参考: [over.match.best]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

[[maybe_unused]] std::string g(double) {
    return "double";
}
[[maybe_unused]] std::string g(int) {
    return "int";
}

[[maybe_unused]] std::string p(long) {
    return "long";
}
[[maybe_unused]] std::string p(double) {
    return "double";
}

[[maybe_unused]] std::string only(int x) {
    return "only:" + std::to_string(x);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D5 best viable function ===\n";

    // char 提升到 int 优于转换到 double
    assert(g('a') == "int");
    [[maybe_unused]] short s = 5;
    assert(g(s) == "int");
    assert(g(1.0) == "double");
    assert(g(1) == "int");

    // int→long 与 int→double 同级 → 歧义（不调用 p(5)）
    assert(p(5L) == "long");
    assert(p(5.0) == "double");

    assert(only(3) == "only:3");

    std::cout << "  exact > promotion > conversion; tie => ambiguous\n";
    std::cout << "best_viable_function: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/best_viable_function", run>;

}  // namespace
