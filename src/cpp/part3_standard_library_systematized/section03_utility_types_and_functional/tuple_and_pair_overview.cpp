// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 tuple / pair
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : tuple_and_pair_overview
// Topic id : part3/section03/tuple_and_pair_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/tuple
//   https://en.cppreference.com/w/cpp/utility/pair

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <utility>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/tuple_and_pair_overview ===\n";

    std::pair<std::string, int> p{"age", 30};
    assert(p.first == "age" && p.second == 30);
    auto p2 = std::make_pair(std::string{"x"}, 1);
    assert(p2.first == "x");

    std::tuple<int, double, std::string> t{1, 2.5, "hi"};
    assert(std::get<0>(t) == 1);
    assert(std::get<double>(t) == 2.5);
    assert(std::get<2>(t) == "hi");
    assert(std::tuple_size_v<decltype(t)> == 3);

    // tie: 解包到已有变量; ignore 跳过
    int i = 0;
    std::string s;
    std::tie(i, std::ignore, s) = t;
    assert(i == 1 && s == "hi");

    // tuple_cat / forward_as_tuple
    [[maybe_unused]] auto cat = std::tuple_cat(std::tuple{1, 2}, std::tuple{3});
    assert(std::get<2>(cat) == 3);

    // pairwise 比较: 字典序
    assert((std::tuple{1, 2} < std::tuple{1, 3}));

    std::cout << "[intro] pair/tuple get/tie ok\n";
    std::cout << "[expert] prefer structured bindings over tie in new code\n";
    std::cout << "tuple_and_pair_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/tuple_and_pair_overview", run>;

}  // namespace
