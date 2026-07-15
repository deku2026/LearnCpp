// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A9 成员指针)
// Stage    : part6_branch_a_object_model
// Section  : section05_misc
// Item     : member_pointers
// Topic id : part6/a/section05/member_pointers
//
// 要点: 数据成员指针 ≈ 对象内偏移; obj.*pm / ptr->*pm;
//       成员函数指针更重: 虚/非虚 + this 调整 (Itanium 常 {ptr, adj} 16 字节);
//       与普通函数指针不可混用; 多继承时偏移/调整更明显。
// 参考: [expr.mptr.oper] [dcl.mptr] Itanium member pointers

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

struct S {
    int a = 10;
    int b = 20;
    double d = 3.5;

    int sum() const { return a + b; }
    virtual int virt() const { return a; }
    virtual ~S() = default;
};

struct BaseX {
    int x = 1;
    virtual int f() const { return x; }
    virtual ~BaseX() = default;
};

struct BaseY {
    int y = 2;
    virtual int g() const { return y; }
    virtual ~BaseY() = default;
};

struct MI : BaseX, BaseY {
    int z = 3;
    int f() const override { return x + z; }
    int g() const override { return y + z; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== member pointers ===\n";

    // --- 数据成员指针 ---
    int S::* pm_a = &S::a;
    int S::* pm_b = &S::b;
    double S::* pm_d = &S::d;

    S s;
    assert(s.*pm_a == 10);
    assert(s.*pm_b == 20);
    assert(s.*pm_d == 3.5);

    S* ps = &s;
    assert(ps->*pm_a == 10);
    ps->*pm_b = 99;
    assert(s.b == 99);

    // 可重新绑定“哪一个成员”
    int S::* pm = pm_a;
    assert(s.*pm == 10);
    pm = pm_b;
    assert(s.*pm == 99);

    // 空成员指针
    int S::* pm_null = nullptr;
    assert(pm_null == nullptr);

    std::cout << "  sizeof(int S::*)=" << sizeof(int S::*) << "  (often pointer-sized offset storage)\n";
    std::cout << "  sizeof(void*)=" << sizeof(void*) << '\n';

    // --- 成员函数指针: 非虚 ---
    int (S::*psum)() const = &S::sum;
    assert((s.*psum)() == s.a + s.b);
    assert((ps->*psum)() == s.a + s.b);

    // --- 成员函数指针: 虚 — 经 vtable 分派 ---
    int (S::*pvirt)() const = &S::virt;
    assert((s.*pvirt)() == s.a);

    struct D : S {
        int virt() const override { return 1000 + a; }
    };
    D der;
    S& as_s = der;
    assert((as_s.*pvirt)() == 1000 + der.a);  // 动态分派

    std::cout << "  sizeof(int (S::*)() const)=" << sizeof(pvirt) << "  (often 2x pointer: addr-or-vslot + this-adj)\n";
#if defined(_MSC_VER)
    // MSVC 单继承成员函数指针可能是 1 个指针; 多继承/虚继承更大
    assert(sizeof(pvirt) >= sizeof(void*));
#else
    // Itanium 典型 16
    assert(sizeof(pvirt) >= sizeof(void*));
#endif

    // --- 多继承: 成员指针带 this 调整信息 ---
    int (BaseY::*pg)() const = &BaseY::g;
    MI mi;
    BaseY* py = &mi;
    assert((py->*pg)() == mi.y + mi.z);

    // 指向 MI 覆盖版本
    int (MI::*pmi_g)() const = &MI::g;
    assert((mi.*pmi_g)() == mi.y + mi.z);

    // 数据成员在 MI 中的偏移
    int MI::* pm_z = &MI::z;
    assert(mi.*pm_z == 3);
    int BaseX::* pm_x = &BaseX::x;
    assert(mi.*pm_x == 1);

    // 成员指针不是普通指针: 不能转 void* 当函数地址用
    // void* raw = (void*)psum;  // 无意义/不可移植

    // 应用场景: 属性表、序列化字段映射、信号槽式成员绑定
    struct Field {
        const char* name;
        int S::* member;
    };
    const Field fields[] = {
        {"a", &S::a},
        {"b", &S::b},
    };
    s.a = 1;
    s.b = 2;
    int total = 0;
    for (const auto& f : fields) {
        total += s.*f.member;
        std::cout << "  field " << f.name << "=" << s.*f.member << '\n';
    }
    assert(total == 3);

    std::cout << "member_pointers: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section05/member_pointers", run>;

}  // namespace
