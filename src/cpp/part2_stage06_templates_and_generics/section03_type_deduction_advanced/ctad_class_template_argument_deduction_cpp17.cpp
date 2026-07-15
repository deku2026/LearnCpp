// Topic    : CTAD —— 类模板实参推导（C++17）
// Doc      : 第2部分-阶段6 · 步骤 8.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : ctad_class_template_argument_deduction_cpp17
// Topic id : part2/stage06/section03/ctad_class_template_argument_deduction_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
//            ISO [over.match.class.deduct]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 标准库 CTAD：pair / vector / lock_guard
// ---------------------------------------------------------------------------

template <typename T, typename U>
struct PairLite {
    T first;
    U second;
    PairLite(T f, U s) : first(std::move(f)), second(std::move(s)) {}
};

// ---------------------------------------------------------------------------
// §进阶 — 自定义类的隐式推导指引（来自构造函数）
// ---------------------------------------------------------------------------

template <typename T>
struct Wrapper {
    T value;
    explicit Wrapper(T v) : value(std::move(v)) {}
};

template <typename T, typename U>
struct AggregateDuo {
    T a;
    U b;
};

// C++17 起聚合也可 CTAD（有时需推导指引，见 deduction_guides topic）

// ---------------------------------------------------------------------------
// §专家 — 全写或全推；空列表/部分列表非法
// ---------------------------------------------------------------------------

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ctad] 入门：标准库 ===\n";
    std::pair p{1, 2.5};  // pair<int,double>
    static_assert(std::is_same_v<decltype(p), std::pair<int, double>>);
    assert(p.first == 1 && p.second == 2.5);

    std::vector v{1, 2, 3};  // vector<int>
    static_assert(std::is_same_v<decltype(v), std::vector<int>>);
    assert(v.size() == 3);

    std::mutex m;
    std::lock_guard lk{m};  // lock_guard<mutex>
    static_assert(std::is_same_v<decltype(lk), std::lock_guard<std::mutex>>);
    std::cout << "pair/vector/lock_guard CTAD OK\n";

    std::cout << "=== 进阶：用户类隐式指引 ===\n";
    PairLite pl{std::string{"k"}, 42};  // PairLite<string,int>
    static_assert(std::is_same_v<decltype(pl), PairLite<std::string, int>>);
    assert(pl.first == "k" && pl.second == 42);

    Wrapper w{3.14};  // Wrapper<double>
    static_assert(std::is_same_v<decltype(w), Wrapper<double>>);
    assert(w.value == 3.14);

    // 也可显式写出
    [[maybe_unused]] std::pair<int, int> explicit_p{1, 2};
    assert(explicit_p.second == 2);
    std::cout << "user PairLite/Wrapper OK\n";

    std::cout << "=== 专家：要么全推导要么全写 ===\n";
    // std::pair<> q{1,2};     // 错误：给了空模板实参列表
    // std::pair<int> r{1,2};  // 错误：只给一部分
    std::pair full{1, 2};  // OK 全推导
    static_assert(std::is_same_v<decltype(full), std::pair<int, int>>);

    // CTAD 只影响「要构造的类型」；类型确定后再做重载决议选构造函数
    std::vector from_list{std::string{"a"}, std::string{"b"}};
    static_assert(std::is_same_v<decltype(from_list), std::vector<std::string>>);
    assert(from_list[0] == "a");
    std::cout << "=== ctad_class_template_argument_deduction_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage06/section03/ctad_class_template_argument_deduction_cpp17", run>;

}  // namespace
