// Topic     : 成员、方法、访问控制
// Doc       : 第2部分-阶段4 · 步骤 1.1
// cppreference: https://en.cppreference.com/cpp/language/classes
//
// 要点: class 默认 private / struct 默认 public；public 接口 + private 数据保护不变量；
//       const 成员函数可在 const 对象上调用；protected 供派生类访问。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Account {
public:
    explicit Account(double initial) : balance_(initial) {}

    void deposit(double amt) {
        assert(amt >= 0.0);
        balance_ += amt;
    }

    bool withdraw(double amt) {
        if (amt < 0.0 || amt > balance_) {
            return false;
        }
        balance_ -= amt;
        return true;
    }

    // const 成员函数：承诺不修改对象可见状态 → 可在 const 对象上调用
    double balance() const { return balance_; }

protected:
    // 派生类可见，外部仍不可见
    void apply_fee(double fee) { balance_ -= fee; }

private:
    double balance_;  // 不变量：由公共接口维护
};

struct Point {  // struct 默认 public
    int x;
    int y;
    int sum() const { return x + y; }
};

class NamedAccount : public Account {
public:
    NamedAccount(std::string name, double initial) : Account(initial), name_(std::move(name)) {}

    void monthly_fee() { apply_fee(1.0); }  // 可访问 protected

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [members_and_access_control] ===\n";

    // -------------------------------------------------------------------------
    // §入门：封装 + 公共接口
    // -------------------------------------------------------------------------
    Account a{100.0};
    a.deposit(25.0);
    assert(a.withdraw(40.0));
    assert(a.balance() == 85.0);
    assert(!a.withdraw(1000.0));  // 余额不足
    // a.balance_ = 0;            // ❌ private，外部不可访问
    std::cout << "[intro] balance after ops=" << a.balance() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：const 方法 + class/struct 默认访问
    // -------------------------------------------------------------------------
    const Account ca{10.0};
    assert(ca.balance() == 10.0);
    // ca.deposit(1.0);           // ❌ 非 const 方法不能在 const 对象上调用

    Point p{3, 4};
    assert(p.sum() == 7);  // struct 成员默认 public，可直接访问 p.x
    assert(p.x + p.y == 7);
    std::cout << "[advanced] const method on const object; Point sum=" << p.sum() << '\n';

    // -------------------------------------------------------------------------
    // §专家：protected 与派生类；不变量边界
    // -------------------------------------------------------------------------
    NamedAccount na{"alice", 50.0};
    na.monthly_fee();
    assert(na.balance() == 49.0);
    assert(na.name() == "alice");
    // na.apply_fee(1.0);         // ❌ protected：外部（非成员）不可调

    // class 与 struct 仅默认访问级别不同；可互相继承（风格上 class 偏封装，struct 偏聚合数据）
    static_assert(sizeof(Point) >= 2 * sizeof(int));

    std::cout << "[expert] protected fee applied; name=" << na.name() << '\n';
    std::cout << "=== members_and_access_control: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/members_and_access_control", run>;

}  // namespace
