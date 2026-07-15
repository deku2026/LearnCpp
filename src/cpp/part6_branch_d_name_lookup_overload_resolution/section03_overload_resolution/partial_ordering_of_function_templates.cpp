// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D6 partial ordering)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : partial_ordering_of_function_templates
// Topic id : part6/d/section03/partial_ordering_of_function_templates
//
// 要点: 更特化（接受更少实参形态）的函数模板胜出。
// 参考: [temp.func.order]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

template <typename T>
std::string k(T) {
    return "k(T)";
}

template <typename T>
std::string k(T*) {
    return "k(T*)";
}

template <typename T>
std::string k(const T*) {
    return "k(const T*)";
}

template <typename T, typename U>
std::string pair_f(T, U) {
    return "pair general";
}

template <typename T>
std::string pair_f(T, T) {
    return "pair same";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D6 partial ordering of function templates ===\n";

    [[maybe_unused]] int x = 1;
    [[maybe_unused]] const int cx = 2;
    assert(k(x) == "k(T)");
    assert(k(&x) == "k(T*)");
    assert(k(&cx) == "k(const T*)");

    assert(pair_f(1, 2.0) == "pair general");
    assert(pair_f(1, 2) == "pair same");

    std::cout << "partial_ordering_of_function_templates: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/partial_ordering_of_function_templates", run>;

}  // namespace
