// Topic     : 流插入/提取运算符
// Doc       : 第2部分-阶段4 · 步骤 6.2
// cppreference: https://en.cppreference.com/cpp/language/operators
//
// 要点: << / >> 必须非成员（左操作数是 iostream）；常 friend 访问私有；返回流引用支持链式。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace {

class Money {
    long cents_ = 0;

public:
    Money() = default;
    explicit Money(long cents) : cents_(cents) {}
    long cents() const { return cents_; }

    friend std::ostream& operator<<(std::ostream& os, const Money& m);
    friend std::istream& operator>>(std::istream& is, Money& m);
};

std::ostream& operator<<(std::ostream& os, const Money& m) {
    const long neg = m.cents_ < 0 ? -1 : 1;
    const long c = m.cents_ * neg;
    const long dollars = c / 100;
    const long rem = c % 100;
    if (neg < 0) {
        os << '-';
    }
    os << '$' << dollars << '.';
    if (rem < 10) {
        os << '0';
    }
    os << rem;
    return os;
}

std::istream& operator>>(std::istream& is, Money& m) {
    // 简化：读一个整数当作分
    long c = 0;
    if (is >> c) {
        m.cents_ = c;
    }
    return is;
}

struct Point {
    int x = 0;
    int y = 0;
};

// 无 private 时可不 friend
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << '(' << p.x << ',' << p.y << ')';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [stream_insertion_extraction] ===\n";

    // -------------------------------------------------------------------------
    // §入门：<< 格式化输出
    // -------------------------------------------------------------------------
    Money m{1234};
    std::ostringstream oss;
    oss << m;
    assert(oss.str() == "$12.34");
    std::cout << "[intro] " << m << '\n';

    // -------------------------------------------------------------------------
    // §进阶：链式 + 负值
    // -------------------------------------------------------------------------
    Money a{100}, b{-250};
    std::ostringstream oss2;
    oss2 << a << ' ' << b;
    assert(oss2.str() == "$1.00 $-2.50");
    Point p{3, 4};
    std::ostringstream oss3;
    oss3 << p;
    assert(oss3.str() == "(3,4)");
    std::cout << "[advanced] chain / Point " << p << '\n';

    // -------------------------------------------------------------------------
    // §专家：>> 提取
    // -------------------------------------------------------------------------
    std::istringstream iss("999");
    Money in;
    iss >> in;
    assert(in.cents() == 999);
    std::cout << "[expert] extracted cents=" << in.cents() << '\n';
    std::cout << "=== stream_insertion_extraction: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/stream_insertion_extraction", run>;

}  // namespace
