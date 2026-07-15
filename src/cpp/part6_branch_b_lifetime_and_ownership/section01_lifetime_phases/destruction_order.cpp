// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B1 销毁顺序)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : destruction_order
// Topic id : part6/b/section01/destruction_order
//
// 要点: 销毁与构造相反（LIFO）：局部、成员、派生→基类。
// 参考: [class.dtor] [stmt.jump]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::vector<std::string> g_log;

struct Track {
    const char* name;
    explicit Track(const char* n) : name(n) { g_log.emplace_back(std::string("c:") + n); }
    ~Track() { g_log.emplace_back(std::string("d:") + name); }
};

struct Base {
    Track b{"Base"};
};

struct Member {
    Track m{"Member"};
};

struct Derived : Base {
    Member mem;
    Track d{"Derived"};
};

// 多个成员：声明序构造，逆序析构
struct Multi {
    Track a{"M.a"};
    Track b{"M.b"};
    Track c{"M.c"};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B1 destruction order ===\n";
    g_log.clear();

    {
        Track a{"A"};
        Track b{"B"};
        Track c{"C"};
    }
    assert((g_log == std::vector<std::string>{"c:A", "c:B", "c:C", "d:C", "d:B", "d:A"}));
    std::cout << "  locals LIFO: OK\n";

    g_log.clear();
    {
        Derived obj;
        (void)obj;
    }
    assert(g_log.front() == "c:Base");
    assert(g_log.back() == "d:Base");
    auto find = [](const std::string& s) {
        for (std::size_t i = 0; i < g_log.size(); ++i)
            if (g_log[i] == s) return i;
        return g_log.size();
    };
    assert(find("d:Derived") < find("d:Member"));
    assert(find("d:Member") < find("d:Base"));
    assert(find("c:Base") < find("c:Member"));
    assert(find("c:Member") < find("c:Derived"));
    std::cout << "  members/bases reverse of construction: OK\n";

    g_log.clear();
    {
        Multi m;
        (void)m;
    }
    assert(find("d:M.c") < find("d:M.b"));
    assert(find("d:M.b") < find("d:M.a"));

    for (const auto& e : g_log) std::cout << "    " << e << '\n';

    std::cout << "  static objects: reverse of completion order at exit (per TU)\n";
    std::cout << "destruction_order: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/destruction_order", run>;

}  // namespace
