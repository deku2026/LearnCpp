// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2.4 未修边界 / B12)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : gaps_not_yet_fixed_in_cpp23
// Topic id : part6/b/section02/gaps_not_yet_fixed_in_cpp23
//
// 要点: P2718 只修 range-for；拆出 auto&& r = get().items 等仍可能悬垂。
// 本 topic 仅安全对照，不访问悬垂引用。
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

const std::string& first_of(const std::vector<std::string>& v) {
    return v.front();
}

// 参数不延长到函数外
const std::string& id_str(const std::string& s) {
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 gaps not fixed by P2718 ===\n";

    // --- 入门: 安全 —— 整对象绑定再访问成员 ---
    {
        const Box& keep = make_box();
        assert(Box::live == 1);
        assert(keep.data().size() == 3);
        [[maybe_unused]] int n = 0;
        for (const auto& s : keep.data()) {
            ++n;
            (void)s;
        }
        assert(n == 3);
    }
    assert(Box::live == 0);

    // --- 进阶: 仍危险的模式（注释，勿解引用）---
    // auto&& r = make_box().data();
    // // make_box() 临时在 full-expression 末尾销毁 → r 可能悬垂
    // for (auto& x : r) { ... }  // UB —— P2718 不修这种「拆出去」的写法

    // 对照：先按值拥有父对象
    {
        auto parent = make_box();
        [[maybe_unused]] auto&& r = parent.data();
        assert(r.size() == 3);
        assert(r[0] == "a");
    }

    // 函数返回引用到临时参数
    // const std::string& d = first_of(std::vector<std::string>{"x"}); // 悬垂
    {
        std::vector<std::string> owned{"x", "y"};
        [[maybe_unused]] const std::string& f = first_of(owned);
        assert(f == "x");
    }

    // 参数传递临时：不延长到调用外
    // const std::string& d2 = id_str(std::string("tmp")); // 悬垂
    {
        std::string owned = "live";
        [[maybe_unused]] const std::string& r = id_str(owned);
        assert(r == "live");
    }

    // 返回局部 string 的 string_view —— 统一悬垂模型；改返回 string
    [[maybe_unused]] auto good = []() -> std::string { return "ok"; };
    assert(good() == "ok");

    // --- 专家: 验收话术 ---
    std::cout << "  P2718 FIXED: temporaries inside range-for range-expression\n";
    std::cout << "  NOT fixed:\n";
    std::cout << "    - auto&& r = getTemp().member; for (x : r)\n";
    std::cout << "    - return ref to temporary / local\n";
    std::cout << "    - parameter binding does not extend past call\n";
    std::cout << "gaps_not_yet_fixed_in_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/gaps_not_yet_fixed_in_cpp23", run>;

}  // namespace
