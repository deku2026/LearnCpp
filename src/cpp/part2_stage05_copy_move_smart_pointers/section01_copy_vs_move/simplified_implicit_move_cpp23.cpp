// Topic    : C++23 简化隐式移动（P2266）—— return 的 move-eligible id 始终是 xvalue
// Doc      : 第2部分-阶段5 · 步骤 4
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : simplified_implicit_move_cpp23
// Topic id : part2/stage05/section01/simplified_implicit_move_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/return
//            https://en.cppreference.com/w/cpp/language/copy_elision
//            P2266R3 Simpler implicit move

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) {}

    Widget(const Widget& o) : name(o.name) { std::cout << "  Widget copy [" << name << "]\n"; }
    Widget(Widget&& o) noexcept : name(std::move(o.name)) { std::cout << "  Widget move [" << name << "]\n"; }
    Widget& operator=(const Widget& o) {
        name = o.name;
        return *this;
    }
    Widget& operator=(Widget&& o) noexcept {
        name = std::move(o.name);
        return *this;
    }
};

// 返回局部对象：C++11 起即有隐式移动（return local 优先选移动）
Widget make_widget() {
    Widget w{"local"};
    return w;  // 隐式移动（或 NRVO）；勿写 return std::move(w);
}

// C++23 P2266：返回右值引用时，move-eligible 的 id-expression 始终是 xvalue
// C++20 对此类「返回引用」场景隐式移动规则不一致/可能失败
Widget&& pass_through(Widget&& w) {
    return w;  // C++23：w 当作 xvalue；合法且按移动语义传递
}

// 对比：返回左值引用时，若 id 被当成 xvalue 则无法绑定到 T&
// 下面函数在「参数是 lvalue 引用」时 return in 仍然 OK（in 不是 rvalue-ref 参数，
// 不一定 move-eligible）。P2266 的经典破坏例是「rvalue-ref 形参 + 返回 lvalue-ref」。
// 此处用显式 static_cast 演示需要左值时的写法：
std::string& as_lvalue(std::string& s) {
    return s;  // 明确返回左值引用，绑定 OK
}

// 若错误地 return 一个 rvalue-ref 形参到 lvalue-ref：
// std::string& bad(std::string&& s) { return s; }
// C++23 中 s 为 xvalue，无法绑定 string& —— 需改设计或显式转换。

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [simplified_implicit_move_cpp23] 入门：return 局部 ===\n";

    Widget a = make_widget();
    assert(a.name == "local");
    std::cout << "make_widget -> " << a.name << '\n';

    std::cout << "=== 进阶：C++23 返回 Widget&& 的 pass-through ===\n";
    {
        Widget src{"pass"};
        Widget dst = pass_through(std::move(src));
        // pass_through 返回 xvalue，初始化 dst 走移动构造
        assert(dst.name == "pass");
        std::cout << "pass_through moved into dst.name=" << dst.name << '\n';
    }

    std::cout << "=== 进阶：隐式移动 vs 显式 std::move ===\n";
    {
        auto make_explicit = []() -> Widget {
            Widget w{"explicit"};
            return w;  // 合法，但可能阻止 NRVO（见 never_move_return_of_local）
        };
        Widget e = make_explicit();
        assert(e.name == "explicit");
        (void)e;
    }

    {
        std::string s = "lv";
        std::string& r = as_lvalue(s);
        assert(&r == &s);
        std::cout << "as_lvalue binds OK: " << r << '\n';
    }

    std::cout << "=== 专家：P2266 修了什么 ===\n";
    // · C++20：隐式移动 = 两次重载决议（先当右值，失败再当左值）；返回引用场景漏掉
    // · C++23：move-eligible id-expression 在 return/co_return/throw 中始终是 xvalue
    //          只做一次决议 → 规则简单，实现一致
    // · 副作用：少数「rvalue-ref 参数 return 到 lvalue-ref」代码在 C++23 会报错
    // · 验收点：讲清「C++23 让返回引用的函数也享受隐式移动」

#if defined(__cpp_lib_three_way_comparison)
    // 仅作编译环境烟雾；隐式移动是语言规则，无专用 feature-test 宏保证
    std::cout << "compiled under modern C++ (expect C++23 mode in this repo)\n";
#endif

    std::cout << "=== simplified_implicit_move_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/simplified_implicit_move_cpp23", run>;

}  // namespace
