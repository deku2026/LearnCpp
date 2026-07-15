// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2.2–B2.3 / B12 验收)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : const_ref_extends_temporary
// Topic id : part6/b/section02/const_ref_extends_temporary
//
// 要点: 临时直接绑到有名 const& / && 时寿命延长到引用；
//       经函数参数/返回/成员子对象绑定路径不「传递」延长。
// 验收: 讲清何时延长、何时不能。
// 参考: [class.temporary]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

struct Guard {
    int id;
    static int live;
    explicit Guard(int i) : id(i) { ++live; }
    ~Guard() { --live; }
};
int Guard::live = 0;

struct Bundle {
    Guard g;
    explicit Bundle(int i) : g(i) {}
};

// 参数绑定不把临时寿命延长到调用外
const Guard& identity(const Guard& g) {
    return g;
}

// 返回局部临时引用 —— 模式上永远错误（示意用 static 代替真 UB）
[[maybe_unused]] const std::string& safe_static_ref() {
    static const std::string keep = "static-ok";
    return keep;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 const_ref extends temporary ===\n";

    // --- 入门: 直接绑定延长 ---
    {
        const Guard& ref = Guard{1};
        assert(Guard::live == 1);
        assert(ref.id == 1);
    }
    assert(Guard::live == 0);

    {
        Guard&& rr = Guard{2};
        assert(Guard::live == 1);
        assert(rr.id == 2);
    }
    assert(Guard::live == 0);

    {
        const std::string& s = std::string("hello");
        assert(s == "hello" && s.size() == 5);
    }

    // --- 进阶: 边界（安全对照，不读悬垂）---
    // ⚠️ const Guard& dangle = identity(Guard{3});
    //    full-expression 结束后临时销毁 → dangle 悬垂
    {
        const Guard& still_named_but_dead = identity(Guard{3});
        (void)still_named_but_dead;
        // 不读取
    }
    assert(Guard::live == 0);

    // ⚠️ 返回临时引用不延长：return Guard{...}; 作为 const Guard& 返回
    assert(safe_static_ref() == "static-ok");

    // --- 专家: 绑到临时的「成员」---
    // const auto& r = Bundle{9}.g;  // 危险：临时 Bundle 可能不延长
    // 安全：先延长整个临时对象
    {
        const Bundle& keep = Bundle{9};
        assert(keep.g.id == 9);
        assert(Guard::live == 1);
    }
    assert(Guard::live == 0);

    // 转发引用 auto&& 绑定 prvalue 也会延长
    {
        auto&& ext = Guard{4};
        assert(ext.id == 4);
        assert(Guard::live == 1);
    }
    assert(Guard::live == 0);

    std::cout << "  extend ONLY when temporary binds directly to named ref\n";
    std::cout << "  NO extend across: return, parameter out, some member paths\n";
    std::cout << "const_ref_extends_temporary: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/const_ref_extends_temporary", run>;

}  // namespace
