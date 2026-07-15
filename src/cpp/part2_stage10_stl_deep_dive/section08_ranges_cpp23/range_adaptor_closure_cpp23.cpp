// Topic     : range_adaptor_closure —— 自定义管道适配器 (C++23, P2387)
// Doc       : 第2部分-阶段10 · 步骤 13.4
// cppreference: https://en.cppreference.com/cpp/ranges/range_adaptor_closure
//
// 要点: 继承 range_adaptor_closure<Me>; operator() 接 viewable_range; 可用 |。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace {

// 取前 2 个
struct Take2 : std::ranges::range_adaptor_closure<Take2> {
    template <std::ranges::viewable_range R>
    constexpr auto operator()(R&& r) const {
        return std::forward<R>(r) | std::views::take(2);
    }
};

// 过滤正数
struct Positives : std::ranges::range_adaptor_closure<Positives> {
    template <std::ranges::viewable_range R>
    constexpr auto operator()(R&& r) const {
        return std::forward<R>(r) | std::views::filter([](int x) { return x > 0; });
    }
};

// 乘以常量(闭包自带状态)
struct Mul : std::ranges::range_adaptor_closure<Mul> {
    int k;
    explicit constexpr Mul(int factor) : k(factor) {}
    template <std::ranges::viewable_range R>
    constexpr auto operator()(R&& r) const {
        return std::forward<R>(r) | std::views::transform([k = k](int x) { return x * k; });
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [range_adaptor_closure_cpp23] ===\n";

    std::vector<int> v{-2, -1, 1, 2, 3, 4};

    // 部分 view 迭代器不能直接喂 vector 双迭代器构造，循环收集
    auto collect = [](auto&& r) {
        std::vector<int> out;
        for (int x : r) out.push_back(x);
        return out;
    };

    auto a = v | Take2{};
    std::vector<int> got_a = collect(a);
    assert((got_a == std::vector<int>{-2, -1}));

    auto b = v | Positives{} | Take2{};
    std::vector<int> got_b = collect(b);
    assert((got_b == std::vector<int>{1, 2}));

    auto c = v | Positives{} | Mul{10} | Take2{};
    std::vector<int> got_c = collect(c);
    assert((got_c == std::vector<int>{10, 20}));

    // 可与标准适配器混用
    auto d = v | Positives{} | std::views::transform([](int x) { return x + 1; });
    std::vector<int> got_d = collect(d);
    assert((got_d == std::vector<int>{2, 3, 4, 5}));

    std::cout << "[range_adaptor_closure] Take2/Positives/Mul pipe OK\n";
    std::cout << "range_adaptor_closure_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/range_adaptor_closure_cpp23", run>;

}  // namespace
