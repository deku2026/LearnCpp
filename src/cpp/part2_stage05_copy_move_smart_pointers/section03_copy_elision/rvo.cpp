// Topic    : RVO / URVO —— 返回 prvalue，直接在调用方返回槽构造
// Doc      : 第2部分-阶段5 · 步骤 5.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : rvo
// Topic id : part2/stage05/section03/rvo
// Refs     : https://en.cppreference.com/w/cpp/language/copy_elision
//            ISO [class.copy.elision]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>

namespace {

int g_ctors = 0;
int g_copies = 0;
int g_moves = 0;

struct Noisy {
    std::string id;
    explicit Noisy(std::string i = "x") : id(std::move(i)) {
        ++g_ctors;
        std::cout << "  ctor " << id << '\n';
    }
    Noisy(const Noisy& o) : id(o.id) {
        ++g_copies;
        std::cout << "  copy " << id << '\n';
    }
    Noisy(Noisy&& o) noexcept : id(std::move(o.id)) {
        ++g_moves;
        o.id = "moved";
        std::cout << "  move " << id << '\n';
    }
    ~Noisy() { std::cout << "  dtor " << id << '\n'; }
};

// URVO / RVO：返回未命名临时（prvalue）
Noisy make_rvo() {
    return Noisy("rvo");
}

Noisy make_rvo_expr(int n) {
    return Noisy(n > 0 ? "pos" : "nonpos");
}

// C++17 起 prvalue 初始化不要求可拷贝/可移动（保证消除）
struct Immovable {
    int v;
    explicit Immovable(int x) : v(x) {}
    Immovable(const Immovable&) = delete;
    Immovable(Immovable&&) = delete;
    Immovable& operator=(const Immovable&) = delete;
    Immovable& operator=(Immovable&&) = delete;
};

Immovable make_immovable() {
    return Immovable{42};  // C++17 保证 RVO：无需拷贝/移动构造
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [rvo] 入门：返回 prvalue ===\n";

    g_ctors = g_copies = g_moves = 0;
    {
        Noisy a = make_rvo();
        assert(a.id == "rvo");
        // C++17 保证：通常只有 1 次构造，0 拷贝 0 移动
        std::cout << "counts: ctors=" << g_ctors << " copies=" << g_copies << " moves=" << g_moves << '\n';
        assert(g_copies == 0);
        // moves 在保证消除下应为 0；若实现额外移动则不强制
    }

    std::cout << "=== 进阶：表达式 prvalue；不可移动类型 ===\n";
    {
        g_ctors = g_copies = g_moves = 0;
        Noisy b = make_rvo_expr(1);
        assert(b.id == "pos");
        assert(g_copies == 0);

        Immovable im = make_immovable();
        assert(im.v == 42);
        std::cout << "Immovable via guaranteed RVO: v=" << im.v << '\n';
    }

    std::cout << "=== 专家：godbolt 技巧 ===\n";
    // · 默认优化：RVO 把对象直接建在调用方栈槽
    // · -fno-elide-constructors（GCC/Clang）可强制显示「被省略的移动」
    // · 注意：C++17 对 prvalue 的「物化」规则下，有些路径即使 -fno-elide 也不再调用移动
    // · 与 NRVO 区别：RVO 返回的是临时 prvalue；NRVO 返回具名局部

    Noisy c = Noisy("direct");  // 同样 prvalue 初始化
    assert(c.id == "direct");

    std::cout << "=== rvo: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/rvo", run>;

}  // namespace
