// Topic    : 可变参数完美转发 —— make_unique / emplace 的核心
// Doc      : 第2部分-阶段6 · 步骤 7.4（接阶段 5）
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : variadic_perfect_forwarding
// Topic id : part2/stage06/section02/variadic_perfect_forwarding
// Refs     : https://en.cppreference.com/w/cpp/utility/forward
//            https://en.cppreference.com/w/cpp/language/parameter_pack
//            ISO [temp.variadic] / [forward]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — my_make_unique：Args&&... + forward 包展开
// ---------------------------------------------------------------------------

template <typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct Point {
    int x = 0;
    int y = 0;
    Point(int x_, int y_) : x(x_), y(y_) { std::cout << "  Point(" << x << ',' << y << ")\n"; }
};

// ---------------------------------------------------------------------------
// §进阶 — 探测值类别：确认左值拷贝、右值移动
// ---------------------------------------------------------------------------

struct Tracker {
    std::string name;
    int copies = 0;
    int moves = 0;

    explicit Tracker(std::string n) : name(std::move(n)) {}
    Tracker(const Tracker& o) : name(o.name), copies(o.copies + 1), moves(o.moves) {
        std::cout << "  copy " << name << '\n';
    }
    Tracker(Tracker&& o) noexcept : name(std::move(o.name)), copies(o.copies), moves(o.moves + 1) {
        std::cout << "  move " << name << '\n';
    }
};

template <typename T, typename... Args>
T construct(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

template <typename C, typename... Args>
decltype(auto) emplace_back_like(C& c, Args&&... args) {
    c.emplace_back(std::forward<Args>(args)...);
    return c.back();
}

// ---------------------------------------------------------------------------
// §专家 — 转发到多参数函数；工厂 + 初始化列表边界
// ---------------------------------------------------------------------------

template <typename F, typename... Args>
decltype(auto) invoke_fwd(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

struct Widget {
    Tracker a;
    Tracker b;
    Widget(Tracker a_, Tracker b_) : a(std::move(a_)), b(std::move(b_)) {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [variadic_perfect_forwarding] 入门：make_unique ===\n";
    auto p = my_make_unique<Point>(3, 4);
    assert(p && p->x == 3 && p->y == 4);
    auto s = my_make_unique<std::string>(5, 'z');
    assert(*s == "zzzzz");

    std::cout << "=== 进阶：左值 vs 右值转发 ===\n";
    Tracker live{"L"};
    auto by_copy = construct<Tracker>(live);  // 左值 → 拷贝
    assert(by_copy.copies >= 1);
    auto by_move = construct<Tracker>(Tracker{"R"});  // 右值 → 移动
    assert(by_move.moves >= 1);

    std::vector<std::string> words;
    std::string tmp = "hello";
    emplace_back_like(words, tmp);               // 左值拷贝进 vector
    emplace_back_like(words, std::string{"x"});  // 右值移动
    assert(words.size() == 2 && words[0] == "hello" && words[1] == "x");
    std::cout << "construct / emplace_back_like OK\n";

    std::cout << "=== 专家：invoke + 多成员工厂 ===\n";
    [[maybe_unused]] auto sum = invoke_fwd([](int a, int b, int c) { return a + b + c; }, 1, 2, 3);
    assert(sum == 6);

    Tracker t1{"A"};
    Tracker t2{"B"};
    // 两个左值 → Widget 构造里各拷一次再移入成员
    Widget w = construct<Widget>(t1, t2);
    assert(w.a.name == "A" && w.b.name == "B");

    // forward 包展开语义：forward<Args_i>(args_i)... 逐个保持值类别
    auto w2 = my_make_unique<Widget>(Tracker{"M"}, Tracker{"N"});
    assert(w2->a.name == "M");
    std::cout << "=== variadic_perfect_forwarding: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/variadic_perfect_forwarding", run>;

}  // namespace
