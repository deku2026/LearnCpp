// Topic     : 捕获 this（按指针访问成员）
// Doc       : 第2部分-阶段3 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [this] 捕获当前对象指针；体内可写成员名；对象销毁后调用 → 悬垂；
//       异步/出作用域回调优先 [*this]（C++17）或显式拷贝需要的成员。

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <string>

namespace {

struct Counter {
    int n = 0;
    std::string tag = "c";

    auto make_bump() {
        // 捕获 this：闭包持有 Counter*
        return [this] { ++n; };
    }

    auto make_label() const {
        return [this] { return tag + ":" + std::to_string(n); };
    }

    std::function<int()> make_snapshot_value() const {
        // 只带走需要的成员，避免依赖 this 寿命
        return [n = n] { return n; };
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [capture_this] ===\n";

    // -------------------------------------------------------------------------
    // §入门：成员函数里用 [this]
    // -------------------------------------------------------------------------
    Counter c;
    auto bump = c.make_bump();
    bump();
    bump();
    assert(c.n == 2);
    assert(c.make_label()() == "c:2");
    std::cout << "[intro] [this] lets lambda read/write members via pointer\n";

    // -------------------------------------------------------------------------
    // §进阶：[=] / [&] 在成员函数中也会捕获 this
    // -------------------------------------------------------------------------
    // 历史易错点：成员函数里写 [=] 仍按值捕获 this 指针（不是拷贝整个对象）！
    // C++20 起对「[=] 隐式捕获 this」有弃用警告路径；应显式写 [this] 或 [*this]。
    struct S {
        int x = 5;
        int via_this() const {
            auto f = [this] { return x; };
            return f();
        }
    };
    assert(S{}.via_this() == 5);
    std::cout << "[advanced] prefer explicit [this] or [*this]; [=] captures this pointer\n";

    // -------------------------------------------------------------------------
    // §专家：寿命
    // -------------------------------------------------------------------------
    {
        Counter local;
        local.n = 10;
        auto snap = local.make_snapshot_value();
        assert(snap() == 10);  // 值已拷贝，与 local 无关
    }
    // 若把 [this] lambda 存到 Counter 销毁之后再调用 → UB（见 capture_star_this）。
    std::cout << "[expert] [this] does not extend object lifetime; snapshot members if needed\n";
    std::cout << "=== capture_this: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_this", run>;

}  // namespace
