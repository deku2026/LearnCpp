// Topic     : deducing this 简化 CRTP
// Doc       : 第2部分-阶段4 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/crtp
//
// 要点: 传统 CRTP 需 Base<Derived>；deducing this 让 self 直接为派生类型，基类可非模板。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

// 传统 CRTP
template <class Derived>
struct CrtpBase {
    std::string interface() { return static_cast<Derived*>(this)->impl(); }
};

struct CrtpD : CrtpBase<CrtpD> {
    std::string impl() { return "crtp-D"; }
};

// C++23：基类无需模板参数
struct Base {
    std::string interface(this auto&& self) { return self.impl(); }

    int scaled(this auto&& self, int k) { return self.value() * k; }
};

struct D : Base {
    std::string impl() { return "deduce-D"; }
    int value() const { return 7; }
};

struct E : Base {
    std::string impl() { return "deduce-E"; }
    int value() const { return 3; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [simplified_crtp_via_deducing_this] ===\n";

    // -------------------------------------------------------------------------
    // §入门：新写法普通继承
    // -------------------------------------------------------------------------
    D d;
    assert(d.interface() == "deduce-D");
    std::cout << "[intro] " << d.interface() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：多派生共享同一非模板基
    // -------------------------------------------------------------------------
    E e;
    assert(e.interface() == "deduce-E");
    assert(d.scaled(2) == 14);
    assert(e.scaled(4) == 12);
    std::cout << "[advanced] D scaled=" << d.scaled(2) << " E scaled=" << e.scaled(4) << '\n';

    // -------------------------------------------------------------------------
    // §专家：对照传统 CRTP
    // -------------------------------------------------------------------------
    CrtpD cd;
    assert(cd.interface() == "crtp-D");

    // 仍是静态分发：无 vtable，interface 内直接调到派生 impl
    // 限制：显式对象参数成员不能 virtual（见 deducing_this_restrictions）
    std::cout << "[expert] classic CRTP still works: " << cd.interface() << '\n';
    std::cout << "=== simplified_crtp_via_deducing_this: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/simplified_crtp_via_deducing_this", run>;

}  // namespace
