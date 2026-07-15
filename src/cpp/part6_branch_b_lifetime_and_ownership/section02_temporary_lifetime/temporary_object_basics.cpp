// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2 临时默认寿命 / B12)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : temporary_object_basics
// Topic id : part6/b/section02/temporary_object_basics
//
// 要点: 临时默认活到 full-expression 末尾；同一表达式内可安全使用。
// 验收: 讲清临时何时被延长、何时不能（本文件覆盖「默认寿命」基线）。
// 参考: [class.temporary]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Probe {
    int id;
    static int live;
    explicit Probe(int i) : id(i) { ++live; }
    Probe(const Probe& o) : id(o.id) { ++live; }
    Probe(Probe&& o) noexcept : id(o.id) {
        ++live;
        o.id = -1;
    }
    ~Probe() { --live; }
    int value() const { return id; }
};
int Probe::live = 0;

int sum_temps() {
    // 两个临时在完整表达式末尾才销毁；求值期间都活着
    return Probe{10}.value() + Probe{20}.value();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 temporary object basics ===\n";

    // --- 入门 ---
    assert(Probe::live == 0);
    int s = sum_temps();
    assert(s == 30);
    assert(Probe::live == 0);

    // 临时作函数实参：活到调用所在完整表达式末尾
    auto take = [](const Probe& p) { return p.value(); };
    assert(take(Probe{7}) == 7);
    assert(Probe::live == 0);

    // --- 进阶: 初始化 named 对象 / 移动 ---
    auto make = []() { return std::string("tmp"); };
    std::string owned = make();  // 临时/返回值初始化 named → 拥有
    assert(owned == "tmp");

    std::string joined = std::string("he") + "llo";
    assert(joined == "hello");

    std::vector<int> v = std::vector<int>{1, 2, 3};
    assert(v.size() == 3);

    // 纯右值可移入
    Probe p = Probe{99};
    assert(p.value() == 99);
    assert(Probe::live == 1);
    Probe q = std::move(p);
    assert(q.value() == 99);
    // p 处于有效但未指定状态；不依赖 p.id
    (void)p;

    // --- 专家: full-expression 边界 ---
    // 逗号表达式：临时一般持续到包含它们的完整表达式末尾
    {
        int before = Probe::live;
        int x = (Probe{1}.value(), Probe{2}.value());
        assert(x == 2);
        assert(Probe::live == before);  // 两个临时已销毁
    }

    std::cout << "  default lifetime: end of full-expression\n";
    std::cout << "  binding to named const&/&& extends (see const_ref_extends_temporary)\n";
    std::cout << "temporary_object_basics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/temporary_object_basics", run>;

}  // namespace
