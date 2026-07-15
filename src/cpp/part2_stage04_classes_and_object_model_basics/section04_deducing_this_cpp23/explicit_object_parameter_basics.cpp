// Topic     : deducing this 显式对象参数基础
// Doc       : 第2部分-阶段4 · 步骤 5.1
// cppreference: https://en.cppreference.com/cpp/language/member_functions
// 提案      : P0847R7
//
// 要点: this 写在第一参数前；调用仍是 obj.foo()；体内用 self 访问成员（无 this 指针）；
//       参数类型可写 Explicit& / const Explicit& / auto&& 等。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>

namespace {

struct Implicit {
    int x = 0;
    void bump() { ++x; }  // 隐式 this
    int get() const { return x; }
};

struct Explicit {
    int x = 0;

    void bump(this Explicit& self) { ++self.x; }

    int get(this const Explicit& self) { return self.x; }

    Explicit& set(this Explicit& self, int v) {
        self.x = v;
        return self;
    }

    std::string describe(this const Explicit& self) { return "x=" + std::to_string(self.x); }

    // 转发引用形式：const/值类别由调用对象推导（与 collapse 专题衔接）
    template <class Self>
    auto&& x_ref(this Self&& self) {
        return std::forward<Self>(self).x;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [explicit_object_parameter_basics] ===\n";

    // -------------------------------------------------------------------------
    // §入门：调用语法不变
    // -------------------------------------------------------------------------
    Explicit e;
    e.bump();
    assert(e.get() == 1);
    e.set(10).bump();
    assert(e.get() == 11);
    std::cout << "[intro] " << e.describe() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：const 对象参数
    // -------------------------------------------------------------------------
    [[maybe_unused]] const Explicit ce = e;
    assert(ce.get() == 11);
    // ce.bump(); // ❌ self 需要 Explicit&
    assert(ce.x_ref() == 11);  // const 左值 → const int&
    std::cout << "[advanced] const get ok; non-const method blocked\n";

    // -------------------------------------------------------------------------
    // §专家：对照隐式 this；体内无 this；指针调用
    // -------------------------------------------------------------------------
    Implicit i;
    i.bump();
    assert(i.get() == 1);

    // 显式对象函数体内写 this 非法（教学说明，不编译）：
    // void bad(this Explicit& self) { this->x = 1; }

    // 指针/引用调用同样：e.bump() 不写 e.bump(e)
    Explicit* p = &e;
    p->set(0);
    assert(p->get() == 0);

    e.x_ref() = 5;
    assert(e.get() == 5);

    // 不能 virtual / 不能 static / 不能再叠 const& 限定 —— 见 deducing_this_restrictions
    std::cout << "[expert] call syntax same as classic members; body uses self not this\n";
    std::cout << "=== explicit_object_parameter_basics: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/explicit_object_parameter_basics", run>;

}  // namespace
