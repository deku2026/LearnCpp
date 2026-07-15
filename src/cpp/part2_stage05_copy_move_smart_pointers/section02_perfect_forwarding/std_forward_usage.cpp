// Topic    : std::forward —— 按推导结果有条件保持值类别
// Doc      : 第2部分-阶段5 · 步骤 6.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : std_forward_usage
// Topic id : part2/stage05/section02/std_forward_usage
// Refs     : https://en.cppreference.com/w/cpp/utility/forward
//            Effective Modern C++ Item 23, 25

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

int g_lvalue_hits = 0;
int g_rvalue_hits = 0;

void target(std::string& s) {
    ++g_lvalue_hits;
    s += "!";
    std::cout << "  target(string&) -> " << s << '\n';
}

void target(std::string&& s) {
    ++g_rvalue_hits;
    std::cout << "  target(string&&) -> " << s << '\n';
}

void target(const std::string& s) {
    std::cout << "  target(const string&) -> " << s << '\n';
}

template <class T>
void wrapper_correct(T&& arg) {
    target(std::forward<T>(arg));  // 完美转发
}

template <class T>
void wrapper_broken(T&& arg) {
    target(arg);  // arg 是有名字的 lvalue → 永远走左值/const 左值重载
}

// move 用于「确定的右值引用参数」；forward 用于「转发引用」
void process_rref(std::string&& s) {
    target(std::move(s));  // 类型已是 &&，无条件转右值
}

template <class T>
void process_fwd(T&& s) {
    target(std::forward<T>(s));  // 可能左可能右
}

// 工厂：构造时完美转发
template <class T, class... Args>
T make_thing(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

struct Bundle {
    std::string a;
    std::string b;
    Bundle(std::string x, std::string y) : a(std::move(x)), b(std::move(y)) {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_forward_usage] 入门：保持值类别 ===\n";

    g_lvalue_hits = g_rvalue_hits = 0;
    std::string s = "hi";
    wrapper_correct(s);                  // 左值 → string&
    wrapper_correct(std::string{"rv"});  // 右值 → string&&
    assert(g_lvalue_hits == 1);
    assert(g_rvalue_hits == 1);
    assert(s == "hi!");

    std::cout << "=== 进阶：漏写 forward 的后果 ===\n";
    {
        g_lvalue_hits = g_rvalue_hits = 0;
        std::string t = "x";
        wrapper_broken(t);
        wrapper_broken(std::string{"y"});  // 本应 rvalue，却当 lvalue
        assert(g_lvalue_hits == 2);
        assert(g_rvalue_hits == 0);
        std::cout << "broken wrapper: all hit lvalue overload\n";
    }

    std::cout << "=== 进阶：move vs forward 选用 ===\n";
    {
        process_rref(std::string{"only-rref"});
        std::string u = "fwd";
        process_fwd(u);
        process_fwd(std::string{"fwd-rv"});
    }

    std::cout << "=== 进阶：构造转发 ===\n";
    {
        std::string x = "A";
        auto b = make_thing<Bundle>(x, std::string{"B"});
        assert(b.a == "A" && b.b == "B");
        assert(x == "A");  // 左值按拷贝进构造
        auto c = make_thing<Bundle>(std::string{"C"}, std::string{"D"});
        assert(c.a == "C" && c.b == "D");
        std::cout << "make_thing Bundle OK\n";
    }

    std::cout << "=== 专家：const / 多载决议 ===\n";
    {
        const std::string cs = "const";
        wrapper_correct(cs);  // → const string&
        // Item 25：右值引用参数用 move；转发引用用 forward
        // forward 的 T 必须是推导得到的那份（含引用部分）
    }

    std::cout << "=== std_forward_usage: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/std_forward_usage", run>;

}  // namespace
