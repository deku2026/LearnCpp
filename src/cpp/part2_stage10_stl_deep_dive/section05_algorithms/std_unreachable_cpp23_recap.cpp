// Topic     : std::unreachable —— 标记不可达路径 (C++23, 阶段9 回顾)
// Doc       : 第2部分-阶段10 · 步骤 5.5 / 阶段9
// cppreference: https://en.cppreference.com/cpp/utility/unreachable
//
// 要点: 告诉编译器"不会执行到这里"; 若实际执行 → UB。用于穷尽 switch 等。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

enum class Shape { Circle, Square, Triangle };

// 穷尽三分支; default 标 unreachable, 便于编译器优化 / 警告
std::string name_of(Shape s) {
    switch (s) {
        case Shape::Circle:
            return "circle";
        case Shape::Square:
            return "square";
        case Shape::Triangle:
            return "triangle";
    }
    // 合法枚举值应已被上面覆盖; 走到这里意味着内存损坏或强制 cast
    std::unreachable();
}

int checked_div(int a, int b) {
    if (b == 0) {
        // 调用方契约: b != 0; 用 assert 拦调试, 发布路径可 unreachable
        assert(b != 0);
        std::unreachable();
    }
    return a / b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_unreachable_cpp23_recap] ===\n";

    assert(name_of(Shape::Circle) == "circle");
    assert(name_of(Shape::Square) == "square");
    assert(name_of(Shape::Triangle) == "triangle");

    assert(checked_div(10, 2) == 5);
    assert(checked_div(-9, 3) == -3);

    // 教学: unreachable 不是"抛异常", 也不是 abort ——
    // 是对编译器的承诺。错误使用 = 静默 UB。优先用 enum class + 穷尽 switch。

    std::cout << "[unreachable] exhaustive switch + contract path OK\n";
    std::cout << "std_unreachable_cpp23_recap: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_unreachable_cpp23_recap", run>;

}  // namespace
