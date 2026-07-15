// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 结构化绑定
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : structured_bindings
// Topic id : part3/section03/structured_bindings
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/structured_binding
//
// 要点: 绑定数组/tuple-like/聚合; auto& / auto&& 影响拷贝; 键值遍历 map。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>

namespace {

struct Agg {
    int x;
    double y;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/structured_bindings ===\n";

    // 数组
    int arr[2] = {10, 20};
    auto [a0, a1] = arr;
    assert(a0 == 10 && a1 == 20);

    // pair / tuple
    std::pair p{1, std::string{"one"}};
    auto& [k, val] = p;
    val = "ONE";
    assert(p.second == "ONE");

    std::tuple t{1, 2.5, std::string{"hi"}};
    auto [i, d, s] = t;
    assert(i == 1 && d == 2.5 && s == "hi");

    // 聚合
    Agg g{7, 3.5};
    auto [gx, gy] = g;
    assert(gx == 7 && gy == 3.5);

    // map 遍历
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    int sum = 0;
    for (const auto& [key, value] : m) {
        sum += value;
        assert(!key.empty());
    }
    assert(sum == 3);

    // 注意: auto [x,y] = expr 先物化匿名对象再绑定; 要引用原对象用 auto&
    std::cout << "[intro] array/pair/tuple/aggregate/map bindings ok\n";
    std::cout << "[expert] choose auto vs auto& carefully to avoid copies\n";
    std::cout << "structured_bindings: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/structured_bindings", run>;

}  // namespace
