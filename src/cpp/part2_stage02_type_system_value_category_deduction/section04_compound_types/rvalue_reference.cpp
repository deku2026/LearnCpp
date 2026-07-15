// Topic     : 右值引用 T&&
// Doc       : 第2部分-阶段2 · 步骤 4.2
// cppreference: https://en.cppreference.com/cpp/language/reference
//               https://en.cppreference.com/cpp/utility/move
//
// 要点: T&& 绑定右值（临时 / xvalue）；为移动语义提供重载钩子；
//       具名右值引用变量表达式仍是左值；std::move 只是转成 xvalue。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

std::string sink_copy(const std::string& s) {
    return s;
}

std::string sink_move(std::string&& s) {
    // 可安全“掏空”s（此处直接返回移动）
    return std::move(s);
}

[[maybe_unused]] int overload(const std::string&) {
    return 1;
}
[[maybe_unused]] int overload(std::string&&) {
    return 2;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [rvalue_reference] ===\n";

    // -------------------------------------------------------------------------
    // 入门：绑定右值
    // -------------------------------------------------------------------------
    std::string a = "hello";
    // std::string&& bad = a; // ❌ 不能直接绑左值

    [[maybe_unused]] std::string&& rr1 = std::move(a);  // move(a) 是 xvalue
    assert(rr1 == "hello");                             // move 本身不搬数据，只是转换值类别

    std::string&& rr2 = std::string{"tmp"};  // 临时量
    assert(rr2 == "tmp");
    std::cout << "[intro] T&& binds rvalues (temporaries / xvalues)\n";

    // -------------------------------------------------------------------------
    // 进阶：重载区分 + 具名仍是左值
    // -------------------------------------------------------------------------
    std::string s = "data";
    assert(overload(s) == 1);             // 左值 → const&
    assert(overload(std::move(s)) == 2);  // xvalue → &&
    assert(overload(std::string{"x"}) == 2);

    std::string&& named = std::string{"named"};
    // named 是右值引用类型的变量，但表达式 named 是【lvalue】
    assert(overload(named) == 1);
    assert(overload(std::move(named)) == 2);

    std::string out = sink_move(std::string{"payload"});
    assert(out == "payload");
    std::string kept = "keep";
    auto copied = sink_copy(kept);
    assert(copied == "keep" && kept == "keep");
    std::cout << "[advanced] named rvalue refs are lvalues; move again to cast\n";

    // -------------------------------------------------------------------------
    // 专家：移动钩子；与 Qt 隐式共享对照；转发引用不是普通 T&&
    // -------------------------------------------------------------------------
    std::vector<int> v{1, 2, 3, 4, 5};
    [[maybe_unused]] const auto* data_before = v.data();
    [[maybe_unused]] const std::size_t n_before = v.size();
    std::vector<int> w = std::move(v);  // 移动：转移缓冲所有权
    assert(w.size() == n_before);
    assert(w[0] == 1 && w[4] == 5);
    // 实现通常偷指针（SSO 小字符串等另议）；至少内容正确转移到 w
    assert(w.data() == data_before || (w.size() == 5 && w[2] == 3));
    // 移动后源对象「有效但未指定」：对 vector 的可移植检查是仍可安全调用 empty/size/clear
    // 多数实现置空，但不保证 size()==0 是标准硬性要求——只断言可观察且不崩溃
    (void)v.size();
    v.clear();  // 合法：移后状态仍可赋值/clear
    assert(v.empty());

    // 🔶 Qt 对照（阶段 2 验收点）：隐式共享(COW) vs 移动语义
    // · Qt QString b = a; → 共享底层数据 + 引用计数，写时才复制（多对象可同时只读共享）
    // · 标准 std::string b = std::move(a); → 所有权转移，a 被掏空，不共享
    // 一句话：Qt 用「共享」回避拷贝；标准 C++ 用「转移」回避拷贝。
    std::string shared_story = "Qt: share+refcount+COW";
    std::string move_story = "std: transfer ownership, source emptied";
    assert(shared_story.find("share") != std::string::npos);
    assert(move_story.find("transfer") != std::string::npos);
    std::cout << "[expert] Qt COW=share; C++ move=transfer — different models\n";

    // 转发引用：template<class T> void f(T&&); 或 auto&& —— 见阶段 6 / reference_collapsing
    // 这里的 std::string&& 类型已固定，是纯右值引用（只收右值）。

    int n = 1;
    int&& rrn = static_cast<int&&>(n);  // 等价于 move 的手工写法
    assert(rrn == 1);
    rrn = 5;
    assert(n == 5);

    std::cout << "[expert] && enables move overloads; std::move is static_cast to &&\n";
    std::cout << "=== rvalue_reference: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/rvalue_reference", run>;

}  // namespace
