// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D5 viable)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : viable_functions
// Topic id : part6/d/section03/viable_functions
//
// 要点: viable = 参数个数匹配且每个实参存在隐式转换序列；deleted 仍可 viable。
// 参考: [over.match.viable]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

[[maybe_unused]] std::string h(double) {
    return "double";
}
[[maybe_unused]] std::string h(const char*) {
    return "cstr";
}
// std::string h(int) = delete; // 若存在且最佳 → 硬错误

[[maybe_unused]] std::string pick(int) {
    return "int";
}
[[maybe_unused]] std::string pick(int*) {
    return "int*";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D5 viable functions ===\n";

    // "hi" → const char* 精确；也可转 double? 不，指针不转 double
    assert(h("hi") == "cstr");
    assert(h(3.14) == "double");
    assert(h(2) == "double");  // int→double 转换，cstr 不可行

    [[maybe_unused]] int x = 0;
    assert(pick(x) == "int");
    assert(pick(&x) == "int*");

    // 非 const 左值引用不能绑临时 → 该候选不可行
    // void q(int&); q(1); // ill-formed

    std::cout << "viable_functions: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/viable_functions", run>;

}  // namespace
