// Topic     : 虚析构函数
// Doc       : 第2部分-阶段4 · 步骤 9.3（验收：基类析构为何必须 virtual）
// cppreference: https://en.cppreference.com/cpp/language/destructor
//
// 要点: 经基类指针 delete 派生对象时，基类析构必须 virtual，否则不调派生析构 → 泄漏/UB；
//       unique_ptr<Base> 同样依赖 virtual ~Base；替代：protected 非虚析构禁止经基类 delete。
//       声明 virtual 析构会抑制隐式移动（见 section03/suppression_rules）→ 常 =default 五件套。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace {

std::string g_log;

struct BaseVirt {
    virtual ~BaseVirt() { g_log += "~B"; }
    // 声明虚析构后建议显式处理特殊成员（此处仅演示析构路径，保持可拷贝默认）
    BaseVirt() = default;
    BaseVirt(const BaseVirt&) = default;
    BaseVirt& operator=(const BaseVirt&) = default;
    BaseVirt(BaseVirt&&) = default;
    BaseVirt& operator=(BaseVirt&&) = default;
};

struct DerivedV : BaseVirt {
    explicit DerivedV() { g_log += "+D"; }
    ~DerivedV() override { g_log += "~D"; }
};

// 反面：非虚析构——栈上仍正确，经基类指针 delete 则 UB（本文件不执行 delete 路径）
struct BaseNonVirt {
    ~BaseNonVirt() { g_log += "~NB"; }
};

struct DerivedNV : BaseNonVirt {
    ~DerivedNV() { g_log += "~ND"; }
};

// 另一种安全策略：protected 非虚析构 → 禁止外部 delete Base*
struct BaseProtectedDtor {
protected:
    ~BaseProtectedDtor() = default;

public:
    virtual void f() {}
};

struct Leaf : BaseProtectedDtor {
    ~Leaf() = default;
    void f() override {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [virtual_destructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：virtual 析构顺序正确
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        BaseVirt* p = new DerivedV;
        delete p;  // ~D then ~B
    }
    assert(g_log == "+D~D~B");
    std::cout << "[intro] log=" << g_log << '\n';

    // -------------------------------------------------------------------------
    // §进阶：unique_ptr 默认删除器同样要正确析构链
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        std::unique_ptr<BaseVirt> up = std::make_unique<DerivedV>();
    }
    assert(g_log == "+D~D~B");
    std::cout << "[advanced] unique_ptr path log=" << g_log << '\n';

    // -------------------------------------------------------------------------
    // §专家：栈对象非虚也可；禁止经非虚基类指针 delete；protected 策略
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        DerivedNV stack;
    }
    assert(g_log == "~ND~NB");  // 栈上析构按派生→基类顺序，正确

    // 危险反例（不要在未开 ASan 的教学路径里真删）：
    // BaseNonVirt* p = new DerivedNV; delete p; // 往往只 ~NB → 泄漏/UB

    Leaf leaf;
    leaf.f();
    // BaseProtectedDtor* bp = &leaf; delete bp; // ❌ protected 析构，外部不可 delete

    // 多态基类 + 虚析构后：若未 =default 移动，可能抑制移动（步骤 3.3）
    static_assert(std::is_move_constructible_v<BaseVirt>);
    static_assert(std::has_virtual_destructor_v<BaseVirt>);
    static_assert(!std::has_virtual_destructor_v<BaseNonVirt>);

    std::cout << "[expert] stack non-virt ok; never delete derived via non-virt base*; "
                 "prefer virtual public dtor or protected non-virt\n";
    std::cout << "=== virtual_destructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/virtual_destructor", run>;

}  // namespace
