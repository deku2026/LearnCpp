// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2 const 引用延长)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : const_ref_extends_temporary
// Topic id : part6/b/section02/const_ref_extends_temporary
//
// 要点: 临时直接绑到有名 const& / && 时寿命延长到引用；经函数参数/返回不传递。
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

// 参数绑定不把临时寿命延长到调用外
const Guard& identity(const Guard& g) {
    return g;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 const_ref extends temporary ===\n";

    // ✅ 直接绑定：延长到 ref 作用域
    {
        const Guard& ref = Guard{1};
        assert(Guard::live == 1);
        assert(ref.id == 1);
    }
    assert(Guard::live == 0);

    // ✅ 右值引用同样延长
    {
        Guard&& rr = Guard{2};
        assert(Guard::live == 1);
        assert(rr.id == 2);
    }
    assert(Guard::live == 0);

    // ✅ 字符串临时
    {
        const std::string& s = std::string("hello");
        assert(s == "hello");
        assert(s.size() == 5);
    }

    // ⚠️ 边界（安全对照，不访问悬垂）:
    // const Guard& dangle = identity(Guard{3});
    // // 完整表达式结束后临时销毁，dangle 悬垂 —— 禁止使用
    {
        const Guard& still_ok = identity(Guard{3});
        (void)still_ok;
        // 此处完整表达式已结束；不读取 still_ok
    }
    assert(Guard::live == 0);

    // ⚠️ 返回临时引用也不延长（示意模式，用 static 避免真 UB）
    auto safe_pattern = []() -> const std::string& {
        // 坏代码: return std::string("x");
        static const std::string keep = "static-ok";
        return keep;
    };
    assert(safe_pattern() == "static-ok");

    std::cout << "  extend only when temporary binds directly to named ref\n";
    std::cout << "const_ref_extends_temporary: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/const_ref_extends_temporary", run>;

}  // namespace
