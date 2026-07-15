// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D6 函数模板重载)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : function_template_in_overload
// Topic id : part6/d/section03/function_template_in_overload
//
// 要点: 模板参与重载；非模板优于模板；更特化模板优先。
// 参考: [temp.func.order]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

template <typename T>
std::string h(T) {
    return "h(T)";
}

template <typename T>
std::string h(T*) {
    return "h(T*)";
}

std::string h(int) {
    return "h(int) non-template";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D6 function template in overload set ===\n";

    assert(h(3.0) == "h(T)");
    int x = 0;
    assert(h(&x) == "h(T*)");
    assert(h(1) == "h(int) non-template");  // 非模板优先

    std::cout << "function_template_in_overload: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/function_template_in_overload", run>;

}  // namespace
