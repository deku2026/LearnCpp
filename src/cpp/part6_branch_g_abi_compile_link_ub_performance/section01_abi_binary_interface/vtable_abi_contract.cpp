// LearnCpp topic
// Doc      : 第6部分-支线G · vtable ABI
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : vtable_abi_contract
// Topic id : part6/g/section01/vtable_abi_contract
//
// 要点: 虚函数表布局是 ABI 的一部分; 增删虚函数/改顺序破坏二进制兼容。
// 参考: Itanium C++ ABI vtable; 支线 A

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace {

struct Base {
    virtual ~Base() = default;
    virtual int code() const { return 1; }
    virtual std::string name() const { return "base"; }
};

struct Derived : Base {
    int code() const override { return 2; }
    std::string name() const override { return "derived"; }
};

// 模拟"库边界": 工厂返回基类
std::unique_ptr<Base> make_widget(bool d) {
    if (d) {
        return std::make_unique<Derived>();
    }
    return std::make_unique<Base>();
}

// 若在新版本插入虚函数到 Base 中间, 旧客户端调用槽位会错位 → ABI break
// struct BaseV2 { virtual ~Base(); virtual void new_api(); virtual int code(); ... };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G1 vtable ABI contract ===\n";

    auto b = make_widget(false);
    auto d = make_widget(true);
    assert(b->code() == 1);
    assert(d->code() == 2);
    assert(d->name() == "derived");

    [[maybe_unused]] Base* p = d.get();
    assert(p->code() == 2);  // 动态分派

    std::cout << "  stable ABI tips:\n";
    std::cout << "    - never reorder/remove virtuals; only append at end (still careful)\n";
    std::cout << "    - prefer pimpl + non-virtual public API for stability\n";
    std::cout << "    - same compiler + same flags across shared libs\n";
    std::cout << "vtable_abi_contract: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/vtable_abi_contract", run>;

}  // namespace
