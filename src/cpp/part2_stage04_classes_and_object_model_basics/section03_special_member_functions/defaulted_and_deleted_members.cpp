// Topic     : =default 与 =delete
// Doc       : 第2部分-阶段4 · 步骤 3.2
// cppreference: https://en.cppreference.com/cpp/language/function#Deleted_functions
//
// 要点: =default 请求编译器生成（保持平凡性更易）；=delete 禁止调用（含隐式转换）；
//       可删除普通函数/重载以收窄接口。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

class Widget {
public:
    Widget() = default;
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    Widget(Widget&&) = default;
    Widget& operator=(Widget&&) = default;
    ~Widget() = default;

    explicit Widget(int v) : v_(v) {}
    int get() const { return v_; }

private:
    int v_ = 0;
};

// 删除特定重载，防止窄化/意外类型
void take_double(double x) {
    assert(x >= 0.0);
}
void take_double(float) = delete;
void take_double(int) = delete;

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [defaulted_and_deleted_members] ===\n";

    // -------------------------------------------------------------------------
    // §入门：可移动不可拷贝
    // -------------------------------------------------------------------------
    static_assert(!std::is_copy_constructible_v<Widget>);
    static_assert(std::is_move_constructible_v<Widget>);
    Widget w{10};
    Widget w2{std::move(w)};
    assert(w2.get() == 10);
    // Widget w3{w2}; // ❌ deleted copy
    std::cout << "[intro] move-only Widget value=" << w2.get() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：=default 与平凡性 / 显式默认构造
    // -------------------------------------------------------------------------
    struct Trivial {
        int x;
        Trivial() = default;  // 仍可能保持 trivial（视成员）
    };
    static_assert(std::is_trivially_default_constructible_v<Trivial>);
    static_assert(std::is_trivially_copyable_v<Trivial>);

    // 用户声明空体 ~T() {} 往往不再 trivial；=default 可保留
    struct D {
        ~D() = default;
        int x = 0;
    };
    static_assert(std::is_trivially_destructible_v<D>);
    std::cout << "[advanced] =default can keep triviality\n";

    // -------------------------------------------------------------------------
    // §专家：删除重载收窄 API；不可移动对象
    // -------------------------------------------------------------------------
    take_double(3.14);
    // take_double(1);    // ❌ deleted
    // take_double(1.0f); // ❌ deleted

    static_assert(!std::is_copy_constructible_v<NonCopyable>);
    static_assert(!std::is_move_constructible_v<NonCopyable>);
    NonCopyable nc;
    (void)nc;

    // 也可 delete 运算符 new 强制栈对象等（示意）
    struct StackOnly {
        static void* operator new(std::size_t) = delete;
        static void* operator new[](std::size_t) = delete;
        int x = 1;
    };
    StackOnly s;
    assert(s.x == 1);

    std::cout << "[expert] deleted overloads + non-copyable/movable patterns\n";
    std::cout << "=== defaulted_and_deleted_members: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/defaulted_and_deleted_members", run>;

}  // namespace
