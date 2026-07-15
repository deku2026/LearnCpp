// Topic     : 下标 [] 与调用 () 运算符
// Doc       : 第2部分-阶段4 · 步骤 6.3
// cppreference: https://en.cppreference.com/cpp/language/operators
//
// 要点: [] / () 必须是成员；const 与非 const 成对；() 做仿函数。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

class IntArray {
    std::vector<int> data_;

public:
    explicit IntArray(std::size_t n, int fill = 0) : data_(n, fill) {}

    int& operator[](std::size_t i) {
        if (i >= data_.size()) {
            throw std::out_of_range("IntArray[]");
        }
        return data_[i];
    }

    const int& operator[](std::size_t i) const {
        if (i >= data_.size()) {
            throw std::out_of_range("IntArray[]");
        }
        return data_[i];
    }

    std::size_t size() const { return data_.size(); }
};

class Multiplier {
    int factor_;

public:
    explicit Multiplier(int f) : factor_(f) {}
    int operator()(int x) const { return x * factor_; }
    int operator()(int x, int y) const { return (x + y) * factor_; }
};

// 有状态仿函数
class Accumulator {
    int sum_ = 0;

public:
    int operator()(int x) {
        sum_ += x;
        return sum_;
    }
    int total() const { return sum_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [subscript_and_call_operators] ===\n";

    // -------------------------------------------------------------------------
    // §入门：[] 读写
    // -------------------------------------------------------------------------
    IntArray a{3, 1};
    a[0] = 10;
    a[1] = 20;
    assert(a[2] == 1);
    const IntArray& ca = a;
    assert(ca[0] == 10);
    // ca[0] = 1; // ❌ const 下标返回 const int&
    std::cout << "[intro] a[0]=" << a[0] << '\n';

    // -------------------------------------------------------------------------
    // §进阶：() 仿函数
    // -------------------------------------------------------------------------
    Multiplier times3{3};
    assert(times3(4) == 12);
    assert(times3(1, 2) == 9);
    std::cout << "[advanced] times3(4)=" << times3(4) << '\n';

    // -------------------------------------------------------------------------
    // §专家：有状态 + 越界
    // -------------------------------------------------------------------------
    Accumulator acc;
    assert(acc(5) == 5);
    assert(acc(7) == 12);
    assert(acc.total() == 12);

    bool threw = false;
    try {
        (void)a[99];
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    std::cout << "[expert] accumulator=" << acc.total() << " bounds checked\n";
    std::cout << "=== subscript_and_call_operators: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/subscript_and_call_operators", run>;

}  // namespace
