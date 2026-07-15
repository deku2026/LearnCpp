// Topic     : Rule of 0
// Doc       : 第2部分-阶段4 · 步骤 3.4
// cppreference: https://en.cppreference.com/cpp/language/rule_of_three
//
// 要点: 现代默认——不手写特殊成员；资源交给 string/vector/unique_ptr 等 RAII 成员；
//       编译器生成的逐成员操作自动正确。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// Rule of 0：不声明析构/拷贝/移动
class Person {
    std::string name_;
    std::vector<int> scores_;
    std::unique_ptr<int> data_;

public:
    explicit Person(std::string name, int data = 0) : name_(std::move(name)), data_(std::make_unique<int>(data)) {}

    void add_score(int s) { scores_.push_back(s); }

    const std::string& name() const { return name_; }
    const std::vector<int>& scores() const { return scores_; }
    int data() const { return data_ ? *data_ : 0; }
};

// 纯值类型同样 Rule of 0
struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [rule_of_zero] ===\n";

    // -------------------------------------------------------------------------
    // §入门：可移动组合类型
    // -------------------------------------------------------------------------
    Person a{"alice", 10};
    a.add_score(100);
    a.add_score(95);

    Person b = std::move(a);  // 逐成员移动：string/vector/unique_ptr 各管各的
    assert(b.name() == "alice");
    assert(b.scores().size() == 2);
    assert(b.data() == 10);
    // a 的 unique_ptr 已被移走，处于有效未指定状态
    std::cout << "[intro] moved Person name=" << b.name() << " scores=" << b.scores().size() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：因 unique_ptr 而不可拷贝（成员决定）
    // -------------------------------------------------------------------------
    static_assert(!std::is_copy_constructible_v<Person>);
    static_assert(std::is_move_constructible_v<Person>);
    // Person c = b; // ❌
    Person c = std::move(b);
    assert(c.name() == "alice");
    std::cout << "[advanced] Person copy = deleted via unique_ptr member\n";

    // -------------------------------------------------------------------------
    // §专家：值类型自动全套特殊成员
    // -------------------------------------------------------------------------
    static_assert(std::is_copy_constructible_v<Version>);
    static_assert(std::is_trivially_copyable_v<Version>);
    Version v1{1, 2, 3};
    Version v2 = v1;
    assert(v2.minor == 2);

    // 对比：若手写 ~Person(){} 会抑制移动 —— Rule of 0 避免踩坑
    std::cout << "[expert] Version trivial copy patch=" << v2.patch << '\n';
    std::cout << "=== rule_of_zero: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/rule_of_zero", run>;

}  // namespace
