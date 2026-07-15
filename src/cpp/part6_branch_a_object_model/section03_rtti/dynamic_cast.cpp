// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A8 dynamic_cast)
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : dynamic_cast
// Topic id : part6/a/section03/dynamic_cast
//
// 要点: 运行期沿 vptr→type_info→继承图 检查; 指针失败→nullptr, 引用失败→bad_cast;
//       需要多态源类型 (通常至少一个虚函数); dynamic_cast<void*> 回到完整对象;
//       非免费 — 热路径优先虚函数而非类型分支。
// 参考: [expr.dynamic.cast] cppreference dynamic_cast

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

namespace {

struct Base {
    virtual ~Base() = default;
    virtual std::string tag() const { return "Base"; }
};

struct Derived : Base {
    int secret = 42;
    std::string tag() const override { return "Derived"; }
    void special() const { assert(secret == 42); }
};

struct Sibling : Base {
    std::string tag() const override { return "Sibling"; }
};

struct Unrelated {
    virtual ~Unrelated() = default;
};

// 多继承侧向转换
struct MixA {
    virtual ~MixA() = default;
    int a = 1;
};
struct MixB {
    virtual ~MixB() = default;
    int b = 2;
};
struct MixAB : MixA, MixB {
    int c = 3;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== dynamic_cast ===\n";

    Derived d;
    Sibling s;
    Base* p_ok = &d;
    Base* p_bad = &s;
    Base* p_null = nullptr;

    // --- 指针: 成功 / 失败 / null ---
    Derived* ok = dynamic_cast<Derived*>(p_ok);
    assert(ok != nullptr);
    ok->special();
    assert(ok->tag() == "Derived");

    [[maybe_unused]] Derived* no = dynamic_cast<Derived*>(p_bad);
    assert(no == nullptr);

    [[maybe_unused]] Derived* from_null = dynamic_cast<Derived*>(p_null);
    assert(from_null == nullptr);

    // 向上转换其实不需要 dynamic_cast (static_cast 即可), 但合法
    [[maybe_unused]] Base* up = dynamic_cast<Base*>(&d);
    assert(up == p_ok);

    // --- 引用: 失败抛 std::bad_cast ---
    Base& r_ok = d;
    Base& r_bad = s;
    [[maybe_unused]] Derived& rd = dynamic_cast<Derived&>(r_ok);
    assert(rd.secret == 42);

    [[maybe_unused]] bool threw = false;
    try {
        Derived& boom = dynamic_cast<Derived&>(r_bad);
        (void)boom;
    } catch (const std::bad_cast& ex) {
        threw = true;
        std::cout << "  bad_cast: " << ex.what() << '\n';
    }
    assert(threw);

    // --- dynamic_cast<void*>: 完整对象起点 (多态) ---
    MixAB ab;
    MixB* pb = &ab;
    [[maybe_unused]] void* complete = dynamic_cast<void*>(pb);
    assert(complete == static_cast<void*>(&ab));

    // 侧向: MixB* → MixA* (经完整类型)
    [[maybe_unused]] MixA* pa = dynamic_cast<MixA*>(pb);
    assert(pa != nullptr);
    assert(pa->a == 1);
    assert(dynamic_cast<MixAB*>(pb) == &ab);

    // --- 非多态源: 只能向下到自身关系的 static 部分; 这里用多态 ---
    // 无虚函数的类作 dynamic_cast 源会编译错误 (若目标需 RTTI 检查)

    // 智能指针: 对 raw 做 dynamic_cast, 再接管/观察
    std::unique_ptr<Base> holder = std::make_unique<Derived>();
    if (auto* raw = dynamic_cast<Derived*>(holder.get())) {
        (void)raw;
        assert(raw->secret == 42);
    } else {
        assert(false);
    }

    // 工程建议: 连续 dynamic_cast 分支 → 考虑访问者模式/虚函数表驱动
    [[maybe_unused]] auto describe = [](Base* b) -> std::string {
        if (auto* x = dynamic_cast<Derived*>(b)) {
            return "derived secret=" + std::to_string(x->secret);
        }
        if (dynamic_cast<Sibling*>(b)) {
            return "sibling";
        }
        return "base-ish";
    };
    assert(describe(p_ok).starts_with("derived"));
    assert(describe(p_bad) == "sibling");

    std::cout << "dynamic_cast: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section03/dynamic_cast", run>;

}  // namespace
