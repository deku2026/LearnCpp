// Topic    : CRTP 注入行为 —— mixin：由 == 生成 !=，由 < 生成全套比较
// Doc      : 第2部分-阶段6 · 步骤 12.2 / 扩展练习 7
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_inject_behavior
// Topic id : part2/stage06/section06/crtp_inject_behavior
// Refs     : https://en.cppreference.com/w/cpp/language/crtp
//            Barton–Nackman trick / Boost.Operators 思想

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// ---------------------------------------------------------------------------
// §入门 — EqualityMixin：派生类只写 ==，基类注入 !=
// ---------------------------------------------------------------------------

template <typename D>
struct EqualityMixin {
    friend bool operator!=(const D& a, const D& b) { return !(a == b); }
};

struct UserId : EqualityMixin<UserId> {
    int value = 0;
    explicit UserId(int v) : value(v) {}
    friend bool operator==(const UserId& a, const UserId& b) { return a.value == b.value; }
};

// ---------------------------------------------------------------------------
// §进阶 — Comparable：只写 <，注入 > <= >=
// ---------------------------------------------------------------------------

template <typename D>
struct Comparable {
    friend bool operator>(const D& a, const D& b) { return b < a; }
    friend bool operator<=(const D& a, const D& b) { return !(b < a); }
    friend bool operator>=(const D& a, const D& b) { return !(a < b); }
};

struct Version : Comparable<Version> {
    int major = 0;
    int minor = 0;
    Version(int ma, int mi) : major(ma), minor(mi) {}
    friend bool operator<(const Version& a, const Version& b) {
        if (a.major != b.major) {
            return a.major < b.major;
        }
        return a.minor < b.minor;
    }
    friend bool operator==(const Version& a, const Version& b) { return a.major == b.major && a.minor == b.minor; }
};

// ---------------------------------------------------------------------------
// §专家 — 链式 fluent mixin；实例计数
// ---------------------------------------------------------------------------

template <typename D>
struct Fluent {
    D& self() { return static_cast<D&>(*this); }
    D& set_flag(bool v) {
        self().flag = v;
        return self();
    }
    D& set_name(std::string n) {
        self().name = std::move(n);
        return self();
    }
};

struct Config : Fluent<Config> {
    bool flag = false;
    std::string name;
};

template <typename D>
struct InstanceCounter {
    static inline int live = 0;
    InstanceCounter() { ++live; }
    InstanceCounter(const InstanceCounter&) { ++live; }
    InstanceCounter(InstanceCounter&&) noexcept { ++live; }
    ~InstanceCounter() { --live; }
};

struct Session : InstanceCounter<Session> {
    int id = 0;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [crtp_inject_behavior] 入门：!= 注入 ===\n";
    UserId a{1}, b{2}, c{1};
    assert(a == c);
    assert(a != b);
    std::cout << "UserId equality mixin OK\n";

    std::cout << "=== 进阶：Comparable 全套比较 ===\n";
    Version v1{1, 2}, v2{1, 5}, v3{2, 0};
    assert(v1 < v2);
    assert(v2 > v1);
    assert(v1 <= v2);
    assert(v3 >= v2);
    assert(!(v1 == v2));
    std::cout << "Version comparisons OK\n";

    std::cout << "=== 专家：Fluent + 实例计数 ===\n";
    Config cfg;
    cfg.set_flag(true).set_name("prod");
    assert(cfg.flag && cfg.name == "prod");
    {
        Session s1;
        Session s2 = s1;
        assert(InstanceCounter<Session>::live == 2);
    }
    assert(InstanceCounter<Session>::live == 0);
    // 为何这里 CRTP 优于虚函数？比较/链式接口编译期已知、要零开销内联
    std::cout << "=== crtp_inject_behavior: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_inject_behavior", run>;

}  // namespace
