// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D5 函数模板进重载)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : function_template_in_overload
// Topic id : part6/d/section03/function_template_in_overload
//
// 要点: 函数模板经推导生成候选；与非模板函数一起决议；非模板更优先（同 ICS）。
// 参考: [over.match.best]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>

namespace {

[[maybe_unused]] std::string g(int) {
    return "non-template g(int)";
}

template <typename T>
[[maybe_unused]] std::string g(T) {
    return "template g<T>";
}

template <typename T>
std::string h(T*) {
    return "h(T*)";
}

template <typename T>
std::string h(const T*) {
    return "h(const T*)";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D5 function template in overload set ===\n";

    assert(g(1) == "non-template g(int)");  // 非模板优先
    assert(g(1.5) == "template g<T>");      // 仅模板匹配 double

    [[maybe_unused]] int x = 0;
    [[maybe_unused]] const int y = 0;
    assert(h(&x) == "h(T*)");
    assert(h(&y) == "h(const T*)");

    // 显式指定模板实参也进入候选
    assert(g<int>(2) == "template g<T>");

    std::cout << "  templates deduce → candidates; non-template wins if equal ICS\n";
    std::cout << "function_template_in_overload: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/function_template_in_overload", run>;

}  // namespace
