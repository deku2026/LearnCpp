// Topic     : friend 函数与友元类
// Doc       : 第2部分-阶段4 · 步骤 8.3
// cppreference: https://en.cppreference.com/cpp/language/friend
//
// 要点: friend 授予私有访问；不对称、不传递；节制使用；运算符/工厂常用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Vault;

class Auditor {
public:
    int read_secret(const Vault& v) const;
};

class Vault {
    int secret_ = 42;
    std::string name_ = "main";

    friend class Auditor;
    friend int peek(const Vault&);
    friend Vault make_vault(int, std::string);

public:
    int public_token() const { return secret_ ^ 1; }
};

int peek(const Vault& v) {
    return v.secret_;
}

Vault make_vault(int s, std::string n) {
    Vault v;
    v.secret_ = s;
    v.name_ = std::move(n);
    return v;
}

int Auditor::read_secret(const Vault& v) const {
    return v.secret_;
}

// friend 不传递：Auditor 的「朋友」不能因此访问 Vault
class Stranger {
public:
    // int steal(const Vault& v) { return v.secret_; } // ❌
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [friend_function_class] ===\n";

    // -------------------------------------------------------------------------
    // §入门：友元函数
    // -------------------------------------------------------------------------
    Vault v = make_vault(7, "alpha");
    assert(peek(v) == 7);
    assert(v.public_token() == (7 ^ 1));
    std::cout << "[intro] peek=" << peek(v) << '\n';

    // -------------------------------------------------------------------------
    // §进阶：友元类
    // -------------------------------------------------------------------------
    [[maybe_unused]] Auditor a;
    assert(a.read_secret(v) == 7);
    std::cout << "[advanced] Auditor read_secret=7\n";

    // -------------------------------------------------------------------------
    // §专家：非成员对称运算场景
    // -------------------------------------------------------------------------
    // 典型：operator<< friend，或 ADL 可见的非成员比较。
    // friend 破坏封装 → 优先 public 只读接口；仅在实现紧密耦合时用。
    Stranger s;
    (void)s;
    std::cout << "[expert] friendship non-transitive; keep surface small\n";
    std::cout << "=== friend_function_class: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/friend_function_class", run>;

}  // namespace
