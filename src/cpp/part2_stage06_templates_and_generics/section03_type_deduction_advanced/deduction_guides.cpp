// Topic    : 推导指引 —— 隐式 vs 用户自定义 deduction guide
// Doc      : 第2部分-阶段6 · 步骤 8.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : deduction_guides
// Topic id : part2/stage06/section03/deduction_guides
// Refs     : https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
//            ISO [over.match.class.deduct]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 隐式指引：从构造函数自动生成
// ---------------------------------------------------------------------------

template <typename T>
struct Box {
    T value;
    Box(T v) : value(std::move(v)) {}  // 隐式: Box(T) -> Box<T>
};

// ---------------------------------------------------------------------------
// §进阶 — 迭代器对：构造参数类型 ≠ 元素类型，必须手写指引
// ---------------------------------------------------------------------------

template <typename T>
struct RangeCopy {
    std::vector<T> data;
    template <typename It>
    RangeCopy(It first, It last) : data(first, last) {}
};

// 用户自定义推导指引：从迭代器推导 value_type
template <typename It>
RangeCopy(It, It) -> RangeCopy<typename std::iterator_traits<It>::value_type>;

// 聚合类也常需指引
template <typename T>
struct AggregateWrap {
    T x;
};
template <typename T>
AggregateWrap(T) -> AggregateWrap<T>;

// ---------------------------------------------------------------------------
// §专家 — 指引按值接受（decay）；优先级与显式构造
// ---------------------------------------------------------------------------

template <typename T>
struct DecayDemo {
    T value;
    DecayDemo(T v) : value(std::move(v)) {}
};

// 指引参数按值 → 数组/函数退化为指针
template <typename T>
DecayDemo(T) -> DecayDemo<T>;

template <typename T>
struct PreferLong {
    T value;
    PreferLong(T v) : value(std::move(v)) {}
};

// 强制 int 字面量推成 long 的演示指引
PreferLong(int) -> PreferLong<long>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [deduction_guides] 入门：隐式指引 ===\n";
    Box b{42};
    static_assert(std::is_same_v<decltype(b), Box<int>>);
    Box bs{std::string{"hi"}};
    static_assert(std::is_same_v<decltype(bs), Box<std::string>>);
    assert(bs.value == "hi");
    std::cout << "implicit Box CTAD OK\n";

    std::cout << "=== 进阶：迭代器推导指引 ===\n";
    std::vector<int> src{10, 20, 30};
    RangeCopy rc{src.begin(), src.end()};  // RangeCopy<int>
    static_assert(std::is_same_v<decltype(rc), RangeCopy<int>>);
    assert(rc.data.size() == 3 && rc.data[1] == 20);

    AggregateWrap aw{3.5};
    static_assert(std::is_same_v<decltype(aw), AggregateWrap<double>>);
    assert(aw.x == 3.5);
    std::cout << "RangeCopy + AggregateWrap OK\n";

    std::cout << "=== 专家：decay 与定制映射 ===\n";
    const char name[] = "ctad";
    DecayDemo dd{name};  // T = const char*（数组 decay），不是 const char[5]
    static_assert(std::is_same_v<decltype(dd), DecayDemo<const char*>>);
    assert(std::string{dd.value} == "ctad");

    PreferLong pl{7};  // 走 PreferLong(int) -> PreferLong<long>
    static_assert(std::is_same_v<decltype(pl), PreferLong<long>>);
    assert(pl.value == 7L);

    // 流程：先用构造函数+指引确定类类型，再重载决议选构造函数
    RangeCopy from_ptrs{src.data(), src.data() + src.size()};
    assert(from_ptrs.data.front() == 10);
    std::cout << "=== deduction_guides: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/deduction_guides", run>;

}  // namespace
