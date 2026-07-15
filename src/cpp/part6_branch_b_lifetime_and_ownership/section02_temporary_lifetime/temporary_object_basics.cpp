// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2 临时对象默认寿命)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : temporary_object_basics
// Topic id : part6/b/section02/temporary_object_basics
//
// 要点: 临时默认活到 full-expression 末尾；可在同一表达式内安全使用。
// 参考: [class.temporary]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Probe {
    int id;
    static int live;
    explicit Probe(int i) : id(i) { ++live; }
    Probe(const Probe& o) : id(o.id) { ++live; }
    ~Probe() { --live; }
    int value() const { return id; }
};
int Probe::live = 0;

int sum_temps() {
    // 两个临时在完整表达式末尾才销毁；表达式求值期间都活着
    return Probe{10}.value() + Probe{20}.value();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 temporary object basics ===\n";

    assert(Probe::live == 0);
    int s = sum_temps();
    assert(s == 30);
    assert(Probe::live == 0);  // 表达式结束后临时已销毁

    // 临时作为函数实参：在调用完整表达式末尾销毁
    auto take = [](const Probe& p) { return p.value(); };
    assert((take(Probe{7}) == 7));
    assert(Probe::live == 0);

    // 按值返回产生临时，可初始化新对象
    auto make = []() { return std::string("tmp"); };
    std::string owned = make();  // 拷贝/移动到 named 对象
    assert(owned == "tmp");

    // 链式临时：string 临时 + 字面量
    std::string joined = std::string("he") + "llo";
    assert(joined == "hello");

    std::vector<int> v = std::vector<int>{1, 2, 3};  // 临时被移入 v
    assert(v.size() == 3);

    std::cout << "  default lifetime: end of full-expression\n";
    std::cout << "temporary_object_basics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/temporary_object_basics", run>;

}  // namespace
