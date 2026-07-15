// Topic     : 默认比较运算符
// Doc       : 第2部分-阶段4 · 步骤 7.1 / 7.4
// cppreference: https://en.cppreference.com/cpp/language/default_comparisons
//
// 要点: =default 的 <=> 一行生成六比较；按成员声明序字典序；
//       自定义 <=> 不自动给 ==。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <compare>
#include <iostream>
#include <string>

namespace {

struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;
    auto operator<=>(const Version&) const = default;
};

struct Person {
    std::string name;
    int age = 0;
    // 只 default == 也可以
    bool operator==(const Person&) const = default;
};

// 自定义 <=> 必须手写 ==
struct CITag {
    std::string s;

    std::weak_ordering operator<=>(const CITag& rhs) const {
        // 简化：比长度作演示（真实 CI 要逐字符 tolower）
        if (s.size() < rhs.s.size()) {
            return std::weak_ordering::less;
        }
        if (s.size() > rhs.s.size()) {
            return std::weak_ordering::greater;
        }
        return std::weak_ordering::equivalent;
    }

    bool operator==(const CITag& rhs) const { return s.size() == rhs.s.size(); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [defaulted_comparison_operators] ===\n";

    // -------------------------------------------------------------------------
    // §入门：一行六比较
    // -------------------------------------------------------------------------
    [[maybe_unused]] Version a{1, 2, 0}, b{1, 3, 0};
    (void)b;
    assert(a < b);
    assert(a != b);
    assert(a <= a);
    assert((Version{1, 2, 0} == a));
    std::cout << "[intro] defaulted <=> Version ok\n";

    // -------------------------------------------------------------------------
    // §进阶：字典序成员顺序
    // -------------------------------------------------------------------------
    [[maybe_unused]] Version c{1, 2, 9}, d{1, 3, 0};
    (void)d;
    assert(c < d);  // minor 先决，patch 再大也小
    Person p1{"bob", 20}, p2{"bob", 20}, p3{"amy", 30};
    assert(p1 == p2);
    assert(!(p1 == p3));
    std::cout << "[advanced] lex order + defaulted ==\n";

    // -------------------------------------------------------------------------
    // §专家：手写 <=> 不自动 ==
    // -------------------------------------------------------------------------
    CITag t1{"aa"}, t2{"bb"};  // 同长 → == 且 equivalent
    assert(t1 == t2);
    assert((t1 <=> t2) == 0);
    CITag t3{"a"};
    assert(t3 < t1);
    // 若删掉 operator==，仅有自定义 <=> 时 == 不可用（C++20 规则）

    std::cout << "[expert] custom <=> needs separate ==\n";
    std::cout << "=== defaulted_comparison_operators: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/defaulted_comparison_operators", run>;

}  // namespace
