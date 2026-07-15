// Topic    : 别名模板（alias template）：typedef 做不到
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 7.1
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : alias_template
// Topic id : part2/stage02/section07/alias_template
// Refs     : https://en.cppreference.com/cpp/language/type_alias
//            https://en.cppreference.com/cpp/types/remove_reference
//            ISO [temp.alias]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// 基础：参数化容器别名
template <class T>
using Vec = std::vector<T>;

// 固定比较器的 map
template <class K, class V>
using Dict = std::map<K, V>;

// 标准库 _t 后缀本质就是别名模板：
// template<class T> using remove_reference_t = typename remove_reference<T>::type;
template <class T>
using RemoveRef = std::remove_reference_t<T>;

template <class T>
using AddConstPtr = const T*;

// 表达式 SFINAE 友好的「元函数接口」别名（预告）
template <class T>
using IterOf = typename T::iterator;

// 可变参数：函数指针类型生成
template <class R, class... Args>
using FnPtr = R (*)(Args...);

int add(int a, int b) {
    return a + b;
}

// 部分「固定」参数的别名模板
template <class V>
using StringMap = std::map<std::string, V>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [alias_template] 入门：Vec<T> / 标准库 _t ===\n";
    {
        Vec<int> a{1, 2, 3};
        Vec<double> b{1.5, 2.5};
        static_assert(std::is_same_v<Vec<int>, std::vector<int>>);
        static_assert(std::is_same_v<Vec<double>, std::vector<double>>);
        assert(a.size() == 3 && b.size() == 2);

        static_assert(std::is_same_v<RemoveRef<int&>, int>);
        static_assert(std::is_same_v<RemoveRef<int&&>, int>);
        static_assert(std::is_same_v<std::remove_reference_t<const int&>, const int>);

        // typedef 无法写 template<class T> typedef std::vector<T> Vec; —— 语法不允许
        std::cout << "[intro] alias templates parameterize typedef-like names\n";
    }

    std::cout << "=== 进阶：组合变换；Dict / StringMap ===\n";
    {
        using CIP = AddConstPtr<int>;
        static_assert(std::is_same_v<CIP, const int*>);

        Dict<std::string, int> ages{{"Ada", 36}, {"Torvalds", 55}};
        assert(ages["Ada"] == 36);

        StringMap<int> sm{{"x", 1}};
        assert(sm.at("x") == 1);

        // 链式：decay 类组合在 type_traits 里常见
        using D = std::decay_t<const int (&)[3]>;  // const int*
        static_assert(std::is_same_v<D, const int*>);

        std::cout << "[advanced] compose aliases like trait_t pipelines\n";
    }

    std::cout << "=== 专家：不能特化别名模板；用主 traits 类特化 ===\n";
    {
        // 别名模板不可偏特化/全特化。需要分支时：
        // 1) 特化 class template，再 using ... = typename X<T>::type;
        // 2) 或 C++20 后用 requires / 概念约束不同重载
        [[maybe_unused]] FnPtr<int, int, int> fp = add;
        assert(fp(2, 3) == 5);

        Vec<int> v{10, 20};
        using It = IterOf<Vec<int>>;
        static_assert(std::is_same_v<It, std::vector<int>::iterator>);
        [[maybe_unused]] It it = v.begin();
        assert(*it == 10);

        // 变量模板 + 别名模板是现代 traits 双璧：_v 给值，_t 给类型
        static_assert(std::is_integral_v<int>);
        static_assert(std::is_same_v<std::add_pointer_t<int>, int*>);

        std::cout << "[expert] specialize class templates, not alias templates\n";
    }

    std::cout << "[alias_template] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/alias_template", run>;

}  // namespace
