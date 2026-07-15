// Topic     : explicit 转换运算符
// Doc       : 第2部分-阶段4 · 步骤 6.4
// cppreference: https://en.cppreference.com/cpp/language/explicit
//
// 要点: explicit operator T() 禁止隐式转换；contextual conversion 允许 if(x)/!x；
//       explicit operator bool 是安全布尔惯用法。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Fraction {
    int num_ = 0;
    int den_ = 1;

public:
    Fraction(int n, int d) : num_(n), den_(d) { assert(d != 0); }

    explicit operator double() const { return static_cast<double>(num_) / static_cast<double>(den_); }

    explicit operator bool() const { return num_ != 0; }

    int num() const { return num_; }
};

class Handle {
    int id_ = -1;

public:
    Handle() = default;
    explicit Handle(int id) : id_(id) {}
    explicit operator bool() const { return id_ >= 0; }
    int id() const { return id_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [explicit_conversion_operators] ===\n";

    // -------------------------------------------------------------------------
    // §入门：必须 static_cast
    // -------------------------------------------------------------------------
    Fraction half{1, 2};
    // double d = half; // ❌
    double d = static_cast<double>(half);
    assert(d == 0.5);
    std::cout << "[intro] explicit double=" << d << '\n';

    // -------------------------------------------------------------------------
    // §进阶：contextual bool
    // -------------------------------------------------------------------------
    if (half) {
        assert(true);
    }
    Fraction zero{0, 1};
    assert(!zero);

    Handle h{7};
    Handle empty;
    assert(h);
    assert(!empty);
    // int x = h; // ❌ 不会隐式变成 int（只有 explicit bool）
    std::cout << "[advanced] contextual bool handle id=" << h.id() << '\n';

    // -------------------------------------------------------------------------
    // §专家：safe bool 历史与现代
    // -------------------------------------------------------------------------
    // C++98 曾用 void* 转换等避免 integer 提升；C++11 explicit operator bool 终结该问题。
    // if(h) OK；h << 1 / h + 1 不会因为 bool 提升而悄悄通过。
    const bool flag = static_cast<bool>(half);
    assert(flag);

    std::cout << "[expert] safe bool via explicit operator bool\n";
    std::cout << "=== explicit_conversion_operators: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/explicit_conversion_operators", run>;

}  // namespace
