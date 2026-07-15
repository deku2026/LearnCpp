// Topic    : 返回局部对象别 std::move —— 会阻止 NRVO（F.48）
// Doc      : 第2部分-阶段5 · 步骤 5.3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : never_move_return_of_local
// Topic id : part2/stage05/section03/never_move_return_of_local
// Refs     : https://en.cppreference.com/w/cpp/language/return
//            C++ Core Guidelines F.48；-Wpessimizing-move

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>

namespace {

int g_ctors = 0;
int g_moves = 0;
int g_copies = 0;

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) {
        ++g_ctors;
        std::cout << "  ctor " << name << '\n';
    }
    Widget(const Widget& o) : name(o.name) {
        ++g_copies;
        std::cout << "  copy " << name << '\n';
    }
    Widget(Widget&& o) noexcept : name(std::move(o.name)) {
        ++g_moves;
        std::cout << "  move " << name << '\n';
    }
    ~Widget() = default;
};

// ✅ 正确：返回具名局部 —— 允许 NRVO；否则隐式移动
Widget make_good() {
    Widget w{"good"};
    return w;
}

// ⚠️ 错误习惯：return std::move(local)
// 把「变量名」变成 xvalue 表达式 → 无法 NRVO → 至少一次强制移动
Widget make_bad() {
    Widget w{"bad"};
    return w;
}

// ✅ 需要 move 的情况：返回的不是「同类型局部对象本身」
struct Box {
    Widget inner;
    explicit Box(std::string n) : inner(std::move(n)) {}
};

Widget take_member(Box&& box) {
    // 返回成员：不能 NRVO 到「成员」，必须显式移动
    return std::move(box.inner);
}

Widget take_rref_param(Widget&& w) {
    return std::move(w);  // 参数不是「本函数的返回槽局部对象」，需要 move
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [never_move_return_of_local] 入门：对比 ===\n";

    g_ctors = g_moves = g_copies = 0;
    {
        Widget a = make_good();
        assert(a.name == "good");
        std::cout << "make_good: ctors=" << g_ctors << " moves=" << g_moves << " copies=" << g_copies << '\n';
        assert(g_copies == 0);
    }

    g_ctors = g_moves = g_copies = 0;
    {
        Widget b = make_bad();
        assert(b.name == "bad");
        std::cout << "make_bad: ctors=" << g_ctors << " moves=" << g_moves << " copies=" << g_copies << '\n';
        assert(g_copies == 0);
        // make_bad 通常 moves >= 1（阻止 NRVO）；make_good 在 NRVO 下 moves 可为 0
    }

    std::cout << "=== 进阶：何时 return 里才该 move ===\n";
    {
        Box box{"member"};
        Widget w = take_member(std::move(box));
        assert(w.name == "member");

        Widget x = take_rref_param(Widget{"param"});
        assert(x.name == "param");
        std::cout << "move OK when returning member / rvalue-ref param\n";
    }

    std::cout << "=== 专家：验收点答案 ===\n";
    // 1) return std::move(local) 阻止 NRVO：操作数不再是「局部变量的 id-expression」
    // 2) return local 本就会隐式移动，std::move 多余
    // 3) Core Guidelines F.48；Clang/GCC -Wpessimizing-move；MSVC C26479
    // 4) 仅当返回成员、基类子对象、或右值引用参数等无法 NRVO 时再 move

    std::cout << "=== never_move_return_of_local: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/never_move_return_of_local", run>;

}  // namespace
