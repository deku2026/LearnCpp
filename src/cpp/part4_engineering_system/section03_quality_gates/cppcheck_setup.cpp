// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.3.3 cppcheck
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : cppcheck_setup
// Topic id : part4/section03/cppcheck_setup
//
// 要点: 不依赖完整编译的静态分析; 补 clang-tidy。--enable=all 常用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {

// 模拟 cppcheck 能抓的模式(我们用安全代码演示「如何写才过关」)

// nullPointer: 解引用前检查
[[maybe_unused]] int deref_safe(const int* p) {
    if (p == nullptr) {
        return -1;
    }
    return *p;
}

// unreadVariable / unusedVariable
[[maybe_unused]] int compute() {
    int x = 1;
    int y = 2;
    return x + y;  // 都读了
}

// arrayIndexOutOfBounds 的安全版本
[[maybe_unused]] int at_checked(const std::vector<int>& v, std::size_t i) {
    if (i >= v.size()) {
        return 0;
    }
    return v[i];
}

// memleak 风格: 用 RAII
struct Handle {
    explicit Handle(int id) : id_(id) {}
    int id() const { return id_; }

private:
    int id_;
};

// 简化「检查报告」
struct Finding {
    std::string id;
    std::string severity;
};

std::vector<Finding> analyze_snippet_null_deref_unfixed() {
    // 若写成: int* p=nullptr; return *p; → error nullPointer
    return {{"nullPointer", "error"}};
}

std::vector<Finding> analyze_snippet_null_deref_fixed() {
    return {};  // 有检查则清空
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== cppcheck_setup ===\n";

    [[maybe_unused]] int v = 7;
    assert(deref_safe(&v) == 7);
    assert(deref_safe(nullptr) == -1);
    assert(compute() == 3);
    assert(at_checked({1, 2, 3}, 1) == 2);
    assert(at_checked({1, 2, 3}, 9) == 0);

    auto bad = analyze_snippet_null_deref_unfixed();
    auto good = analyze_snippet_null_deref_fixed();
    assert(bad.size() == 1 && bad[0].id == "nullPointer");
    assert(good.empty());
    std::cout << "  simulated finding: " << bad[0].severity << ":" << bad[0].id << '\n';

    Handle h{42};
    assert(h.id() == 42);

    // CLI: cppcheck --enable=all --std=c++23 src/
    [[maybe_unused]] const char* enable = "all";
    assert(std::string_view(enable) == "all");
    std::cout << "  tip: cppcheck --enable=all --std=c++23 (no full compile)\n";

    // 与 clang-tidy 互补: 不同规则引擎, 建议 CI 两者都跑
    std::cout << "cppcheck_setup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/cppcheck_setup", run>;

}  // namespace
