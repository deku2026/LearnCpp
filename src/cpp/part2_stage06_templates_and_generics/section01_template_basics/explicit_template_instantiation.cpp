// Topic    : 显式实例化 —— 强制生成 / extern 抑制隐式实例化
// Doc      : 第2部分-阶段6 · 步骤 5.4
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : explicit_template_instantiation
// Topic id : part2/stage06/section01/explicit_template_instantiation
// Refs     : https://en.cppreference.com/w/cpp/language/class_template
//            ISO [temp.explicit]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 显式实例化定义：在本 TU 强制生成符号
// ---------------------------------------------------------------------------

template <typename T>
struct Widget {
    T value{};
    T twice() const { return value + value; }
};

template <typename T>
T add_one(T x) {
    return x + T{1};
}

// 显式实例化定义（definition）：编译器必须在此生成完整类/函数
template struct Widget<int>;
template int add_one<int>(int);

// ---------------------------------------------------------------------------
// §进阶 — 显式实例化声明（extern）：告诉编译器「别处已有定义」
// ---------------------------------------------------------------------------
// 典型工程：
//   // widget.hpp
//   template<class T> struct Widget { ... };
//   extern template struct Widget<int>;   // 抑制本 TU 隐式实例化
//
//   // widget_inst.cpp
//   #include "widget.hpp"
//   template struct Widget<int>;          // 唯一定义点
//
// 同一 TU 内不能既 definition 又 extern 同一特化 → 这里只保留 definition，
// 并再实例化另一个常用类型作为对照。

template struct Widget<double>;

template <typename T>
struct Heavy {
    static int live_count;
    Heavy() { ++live_count; }
    ~Heavy() { --live_count; }
    T data{};
};

template <typename T>
int Heavy<T>::live_count = 0;

// 只为常用类型显式实例化，冷门类型仍按需隐式实例化
template struct Heavy<double>;

// ---------------------------------------------------------------------------
// §专家 — 显式实例化与特化的区别；成员函数粒度
// ---------------------------------------------------------------------------

template <typename T>
struct Calculator {
    static T square(T x) { return x * x; }
    static T cube(T x) { return x * x * x; }
};

// 可以只显式实例化某个成员
template int Calculator<int>::square(int);

// 类的显式实例化会实例化所有「已定义」成员（调用与否在显式实例化时规则更积极）
template struct Calculator<long>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [explicit_template_instantiation] 入门 ===\n";
    Widget<int> w;
    w.value = 21;
    assert(w.twice() == 42);
    assert(add_one(41) == 42);
    // 未显式实例化的类型仍可隐式实例化
    Widget<std::string> ws;
    ws.value = "a";
    assert(ws.twice() == "aa");
    std::cout << "Widget/add_one explicit + implicit OK\n";

    std::cout << "=== 进阶：控制常用类型代码生成 ===\n";
    {
        Heavy<double> h1;
        Heavy<double> h2;
        assert(Heavy<double>::live_count == 2);
    }
    assert(Heavy<double>::live_count == 0);
    Heavy<int> hi;  // 隐式实例化另一类型
    assert(Heavy<int>::live_count == 1);
    (void)hi;
    std::cout << "Heavy live_count managed OK\n";

    std::cout << "=== 专家：与特化对比；成员级显式实例化 ===\n";
    // 特化 = 提供不同实现；显式实例化 = 强制用主模板生成某类型
    assert(Calculator<int>::square(5) == 25);
    assert(Calculator<long>::cube(3) == 27);
    // 工程取舍：把 vector/string 等重模板的常用实例放到单个 .cpp
    // extern template class std::vector<int>;  // 标准库本身已广泛使用此技巧
    static_assert(std::is_same_v<decltype(add_one(1)), int>);
    std::vector<int> v{1, 2};  // 对照：库侧显式实例化用户无感
    assert(v.size() == 2);
    std::cout << "=== explicit_template_instantiation: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/explicit_template_instantiation", run>;

}  // namespace
