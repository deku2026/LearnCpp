// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2 P2718 未修边界)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : gaps_not_yet_fixed_in_cpp23
// Topic id : part6/b/section02/gaps_not_yet_fixed_in_cpp23
//
// 要点: P2718 只修 range-for；拆出 auto&& r = get().items 仍可能悬垂。
//       本 topic 用安全对照展示正确写法，不访问悬垂引用。
// 参考: [class.temporary] [stmt.ranged]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Box {
    std::vector<std::string> items;
    static int live;
    Box() : items{"a", "b", "c"} { ++live; }
    ~Box() { --live; }
    const std::vector<std::string>& data() const { return items; }
};
int Box::live = 0;

Box make_box() {
    return Box{};
}

// 参数绑定不延长到函数外
const std::string& first_of(const std::vector<std::string>& v) {
    return v.front();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 gaps not fixed by P2718 ===\n";

    // ✅ 安全：整对象绑定，再访问成员
    {
        const Box& keep = make_box();  // 临时延长到 keep
        assert(Box::live == 1);
        assert(keep.data().size() == 3);
        int n = 0;
        for (const auto& s : keep.data()) {
            ++n;
            (void)s;
        }
        assert(n == 3);
    }
    assert(Box::live == 0);

    // ⚠️ 仍危险的模式（注释，勿解引用）：
    // auto&& r = make_box().data();  // 只延长 .data() 的引用绑定？
    // // make_box() 临时在 full-expression 末尾销毁 → r 可能悬垂
    // for (auto& x : r) { ... }     // UB

    // ✅ 对照：先延长父对象
    {
        auto parent = make_box();  // 按值拥有
        auto&& r = parent.data();
        assert(r.size() == 3);
        assert(r[0] == "a");
    }

    // ⚠️ 函数返回引用到临时参数仍危险
    // const std::string& d = first_of(std::vector<std::string>{"x"}); // 悬垂
    // ✅ 对照：拥有容器
    {
        std::vector<std::string> owned{"x", "y"};
        const std::string& f = first_of(owned);
        assert(f == "x");
    }

    // ⚠️ 返回局部 string 的 string_view（统一悬垂模型）—— 改返回 string
    auto good = []() -> std::string { return "ok"; };
    assert(good() == "ok");

    std::cout << "  P2718: only range-for; other temporary rules unchanged\n";
    std::cout << "gaps_not_yet_fixed_in_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/gaps_not_yet_fixed_in_cpp23", run>;

}  // namespace
