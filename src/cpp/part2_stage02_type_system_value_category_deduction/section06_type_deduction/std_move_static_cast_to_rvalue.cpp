// Topic    : std::move = static_cast 到右值引用（不移动对象）
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 5.3 / 4.2
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : std_move_static_cast_to_rvalue
// Topic id : part2/stage02/section06/std_move_static_cast_to_rvalue
// Refs     : https://en.cppreference.com/cpp/utility/move
//            https://en.cppreference.com/cpp/language/value_category
//            《Effective Modern C++》条款 23

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

struct Tracker {
    std::string id;
    int moves = 0;
    int copies = 0;

    Tracker(std::string s) : id(std::move(s)) {}
    Tracker(const Tracker& o) : id(o.id), moves(o.moves), copies(o.copies + 1) {}
    Tracker(Tracker&& o) noexcept : id(std::move(o.id)), moves(o.moves + 1), copies(o.copies) { o.id = "moved-from"; }
    Tracker& operator=(const Tracker& o) {
        id = o.id;
        moves = o.moves;
        copies = o.copies + 1;
        return *this;
    }
    Tracker& operator=(Tracker&& o) noexcept {
        id = std::move(o.id);
        moves = o.moves + 1;
        copies = o.copies;
        o.id = "moved-from";
        return *this;
    }
};

void take_lvalue(const std::string&) {}
void take_rvalue(std::string&&) {}

// 重载：证明值类别如何选中移动路径
[[maybe_unused]] int overload_tag(const Tracker&) {
    return 1;
}
[[maybe_unused]] int overload_tag(Tracker&&) {
    return 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [std_move_static_cast_to_rvalue] 入门：move 只是强制转型 ===\n";
    {
        std::string s = "hello";

        // 典型实现：static_cast<remove_reference_t<T>&&>(t)
        auto&& x1 = std::move(s);
        auto&& x2 = static_cast<std::string&&>(s);
        static_assert(std::is_same_v<decltype(x1), std::string&&>);
        static_assert(std::is_same_v<decltype(x2), std::string&&>);

        // std::move(s) 的结果是 xvalue；真正的「移动」发生在移动构造/赋值里
        static_assert(std::is_same_v<decltype(std::move(s)), std::string&&>);

        take_lvalue(s);
        // take_rvalue(s);  // ❌ 左值不能绑 string&&
        take_rvalue(std::move(s));
        // 注意：此后 s 处于有效但未指定状态；本例不再使用 s 的内容

        std::cout << "[intro] std::move does not move; it produces an xvalue\n";
    }

    std::cout << "=== 进阶：具名右值引用仍是 lvalue；再 move 一次 ===\n";
    {
        Tracker t{"A"};
        Tracker&& rr = std::move(t);  // rr 绑定到 t，表达式类型 Tracker&&
        // 但表达式「rr」是具名对象 → lvalue！
        assert(overload_tag(rr) == 1);             // 选 const Tracker& / 拷贝路径标签 1
        assert(overload_tag(std::move(rr)) == 2);  // 再转成 xvalue → 标签 2

        Tracker t2 = std::move(rr);  // 真正移动构造
        assert(t2.id == "A");
        assert(t2.moves == 1);
        assert(t.id == "moved-from");
        std::cout << "[advanced] named rvalue ref is lvalue; need std::move again\n";
    }

    std::cout << "=== 专家：与 static_cast 等价；const 对象 move 仍可能走拷贝 ===\n";
    {
        const Tracker ct{"const"};
        // move 得到 const Tracker&&；移动构造通常要 Tracker&&，于是回退到拷贝
        Tracker t3 = std::move(ct);
        assert(t3.id == "const");
        assert(t3.copies == 1);
        assert(ct.id == "const");  // 源未被掏空

        int n = 3;
        using MoveT = decltype(std::move(n));
        using CastT = decltype(static_cast<int&&>(n));
        static_assert(std::is_same_v<MoveT, int&&>);
        static_assert(std::is_same_v<CastT, int&&>);

        std::cout << "[expert] prefer std::move for intent; cast is the mechanism\n";
    }

    std::cout << "[std_move_static_cast_to_rvalue] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/std_move_static_cast_to_rvalue", run>;

}  // namespace
