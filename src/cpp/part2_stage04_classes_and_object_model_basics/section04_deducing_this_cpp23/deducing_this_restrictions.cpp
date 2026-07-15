// Topic     : deducing this 限制
// Doc       : 第2部分-阶段4 · 步骤 5.5
// cppreference: https://en.cppreference.com/cpp/language/member_functions
//
// 要点: 不能 static；不能 virtual；不能再加 cv/ref 限定；体内无 this 指针。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

struct Ok {
    int x = 1;

    int get(this const Ok& self) { return self.x; }

    template <class Self>
    auto&& ref(this Self&& self) {
        return std::forward<Self>(self).x;
    }
};

// 非法组合仅注释说明（保证本文件可编译）：
// struct Bad {
//   void f(this Bad& self) const;     // ❌ 不能再 const
//   void g(this Bad& self) &;         // ❌ 不能再 ref-qualifier
//   static void h(this Bad& self);    // ❌ 不能 static
//   virtual void v(this Bad& self);   // ❌ 不能 virtual
//   void w(this Bad& self) { this->x; } // ❌ 无 this 指针
// };

struct PolyBase {
    virtual std::string name() const { return "base"; }
    virtual ~PolyBase() = default;
};

struct PolyDerived : PolyBase {
    // 重写仍用传统隐式 this + virtual
    std::string name() const override { return "derived"; }

    // 非虚的显式对象方法可以存在于同一类
    int tag(this const PolyDerived& self) {
        (void)self;
        return 42;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [deducing_this_restrictions] ===\n";

    // -------------------------------------------------------------------------
    // §入门：合法显式对象成员
    // -------------------------------------------------------------------------
    Ok o;
    assert(o.get() == 1);
    o.ref() = 9;
    assert(o.get() == 9);
    std::cout << "[intro] legal explicit object methods ok\n";

    // -------------------------------------------------------------------------
    // §进阶：多态仍走传统 virtual
    // -------------------------------------------------------------------------
    PolyDerived d;
    PolyBase& b = d;
    assert(b.name() == "derived");
    assert(d.tag() == 42);
    std::cout << "[advanced] virtual stays classic; tag=" << d.tag() << '\n';

    // -------------------------------------------------------------------------
    // §专家：为何不能 virtual
    // -------------------------------------------------------------------------
    // virtual 依赖固定 vtable 槽；template<class Self> void f(this Self&&)
    // 可实例化出无穷多签名，无法放入单一槽位 → 标准直接禁止 virtual+显式对象参数。
    //
    // 为何不能 static：static 无对象，与「对象参数显式化」矛盾。
    //
    // 为何不能再写 const/&：const/值类别已编码在 Self / 参数类型里。

    static_assert(std::is_polymorphic_v<PolyBase>);
    std::cout << "[expert] restrictions documented; polymorphic_v=1\n";
    std::cout << "=== deducing_this_restrictions: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/deducing_this_restrictions", run>;

}  // namespace
