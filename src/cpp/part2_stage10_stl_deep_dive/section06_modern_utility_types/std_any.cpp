// Topic     : std::any —— 任意单类型类型擦除
// Doc       : 第2部分-阶段10 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/utility/any
//
// 要点: any_cast; 错类型抛/返回 nullptr; vs variant(编译期闭合)。

#include "learn/topic_registry.hpp"

#include <any>
#include <cassert>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_any] ===\n";

    std::any a;
    assert(!a.has_value());

    a = 42;
    assert(a.type() == typeid(int));
    assert(std::any_cast<int>(a) == 42);

    a = std::string{"hello"};
    assert(std::any_cast<std::string>(a) == "hello");

    // 指针形式: 失败返回 nullptr, 不抛
    assert(std::any_cast<int>(&a) == nullptr);
    if (auto* p = std::any_cast<std::string>(&a)) {
        (void)p;
        assert(*p == "hello");
    }

    // 引用形式错类型 → 抛
    [[maybe_unused]] bool threw = false;
    try {
        (void)std::any_cast<double>(a);
    } catch (const std::bad_any_cast&) {
        threw = true;
    }
    assert(threw);

    a.reset();
    assert(!a.has_value());

    // 装容器
    a = std::vector<int>{1, 2, 3};
    auto v = std::any_cast<std::vector<int>>(a);
    assert(v.size() == 3 && v[1] == 2);

    // emplace
    a.emplace<std::string>(4, 'a');
    assert(std::any_cast<std::string>(a) == "aaaa");

    std::cout << "[any] cast/pointer-cast/reset/emplace OK\n";
    std::cout << "std_any: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_any", run>;

}  // namespace
