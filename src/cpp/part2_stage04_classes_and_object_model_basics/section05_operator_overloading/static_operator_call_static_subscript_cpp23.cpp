// Topic     : C++23 static operator() / static operator[]
// Doc       : 第2部分-阶段4 · 步骤 6.5
// cppreference: https://en.cppreference.com/cpp/language/operators
// 提案      : P1169 / P2589
//
// 要点: 无状态函数对象的调用/下标可声明 static，无隐式 this，利于优化。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

struct Hash {
    static std::size_t operator()(int x) noexcept { return static_cast<std::size_t>(x) * 2654435761u; }
};

struct Times {
    static int operator()(int x, int y) { return x * y; }
};

// static 下标：像编译期查表/策略标签
struct Latin {
    static char operator[](int i) {
        assert(i >= 0 && i < 26);
        return static_cast<char>('a' + i);
    }
};

// 对比：非 static 仍有 this（哪怕不用）
struct OldHash {
    std::size_t operator()(int x) const noexcept { return static_cast<std::size_t>(x) * 2654435761u; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [static_operator_call_static_subscript_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：static operator()
    // -------------------------------------------------------------------------
    [[maybe_unused]] Hash h;
    assert(h(1) != 0);
    assert(Hash{}(2) == Hash::operator()(2));
    assert(Times{}(6, 7) == 42);
    std::cout << "[intro] Hash(2)=" << Hash{}(2) << " Times=" << Times{}(6, 7) << '\n';

    // -------------------------------------------------------------------------
    // §进阶：static operator[]
    // -------------------------------------------------------------------------
    assert(Latin{}[0] == 'a');
    assert(Latin{}[25] == 'z');
    char c = Latin::operator[](1);
    assert(c == 'b');
    std::cout << "[advanced] Latin[1]=" << c << '\n';

    // -------------------------------------------------------------------------
    // §专家：与对象状态无关；可当 NTTP/策略
    // -------------------------------------------------------------------------
    [[maybe_unused]] OldHash oh;
    assert(oh(3) == Hash{}(3));

    // static 调用运算符不依赖实例：即使类型不可默认构造也可通过 类型名::operator() 调用
    [[maybe_unused]] auto v = Hash::operator()(10);
    assert(v == Hash{}(10));

    std::cout << "[expert] static call without meaningful object state\n";
    std::cout << "=== static_operator_call_static_subscript_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage04/section05/static_operator_call_static_subscript_cpp23", run>;

}  // namespace
