// Topic     : this 指针
// Doc       : 第2部分-阶段4 · 步骤 1.2
// cppreference: https://en.cppreference.com/cpp/language/this
//
// 要点: 非静态成员函数有隐式 this；消歧义参数名与成员；返回 *this 做链式调用；
//       const 方法中 this 是 const T*；deducing this 把隐式参数显式化（步骤 5）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Builder {
    int x_ = 0;
    int y_ = 0;
    std::string name_;

public:
    // this-> 消歧义：参数名与成员同名时
    Builder& set_x(int x) {
        this->x_ = x;
        return *this;  // 返回自身引用 → 链式
    }

    Builder& set_y(int y) {
        this->y_ = y;
        return *this;
    }

    Builder& set_name(std::string name) {
        name_ = std::move(name);  // 无歧义时不必写 this->
        return *this;
    }

    int x() const { return x_; }
    int y() const { return y_; }
    const std::string& name() const { return name_; }

    // 返回 this 的地址（教学演示；真实代码少见）
    const Builder* self() const { return this; }

    // 比较地址：是否是同一个对象
    bool is_same_object(const Builder& other) const { return this == &other; }
};

class Counter {
    int n_ = 0;

public:
    Counter& inc() {
        ++n_;
        return *this;
    }

    // 返回值（拷贝）而不是引用：链断在拷贝上
    Counter next() const {
        Counter c = *this;
        ++c.n_;
        return c;
    }

    int get() const { return n_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [this_pointer] ===\n";

    // -------------------------------------------------------------------------
    // §入门：消歧义 + 链式调用
    // -------------------------------------------------------------------------
    Builder b;
    b.set_x(1).set_y(2).set_name("origin");
    assert(b.x() == 1 && b.y() == 2 && b.name() == "origin");
    std::cout << "[intro] chained builder: (" << b.x() << "," << b.y() << ") " << b.name() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：this 身份 / const 方法中的 this
    // -------------------------------------------------------------------------
    [[maybe_unused]] const Builder& cref = b;
    assert(cref.self() == &b);
    assert(b.is_same_object(b));
    Builder other;
    assert(!b.is_same_object(other));
    // const 方法里 this 类型为 const Builder*，不能写 this->x_ = 0
    std::cout << "[advanced] this as object identity; self()==&b\n";

    // -------------------------------------------------------------------------
    // §专家：返回 *this 引用 vs 按值；与隐式对象参数的关系
    // -------------------------------------------------------------------------
    Counter c;
    c.inc().inc().inc();
    assert(c.get() == 3);

    Counter c2 = c.next();  // next 返回新对象，不修改 c
    assert(c.get() == 3 && c2.get() == 4);

    // 隐式 this 等价于「第一个隐藏参数」：void f(T* this, ...)；
    // C++23 deducing this 把它写成显式参数 this Self&& self。
    std::cout << "[expert] *this ref chains mutate; next() returns copy n=" << c2.get() << '\n';
    std::cout << "=== this_pointer: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/this_pointer", run>;

}  // namespace
