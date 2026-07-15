// Topic    : NRVO —— 具名返回值优化（允许但不保证）
// Doc      : 第2部分-阶段5 · 步骤 5.1–5.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : nrvo
// Topic id : part2/stage05/section03/nrvo
// Refs     : https://en.cppreference.com/w/cpp/language/copy_elision
//            ISO [class.copy.elision]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

int g_ctors = 0;
int g_copies = 0;
int g_moves = 0;

struct Noisy {
    std::string id;
    explicit Noisy(std::string i) : id(std::move(i)) {
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

// 经典 NRVO：单一局部，单一 return
Noisy make_nrvo() {
    Noisy n("nrvo");
    n.id += "!";
    return n;  // 允许 NRVO；否则隐式移动
}

// 多条 return 路径：许多编译器仍可 NRVO，但不保证
Noisy make_branch(bool flag) {
    Noisy a("A");
    Noisy b("B");
    if (flag) {
        return a;
    }
    return b;
}

// 与 RVO 对比：返回 prvalue 是保证消除（C++17）
Noisy make_prvalue() {
    return Noisy("prvalue");
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [nrvo] 入门：具名局部返回 ===\n";

    g_ctors = g_copies = g_moves = 0;
    {
        Noisy x = make_nrvo();
        assert(x.id == "nrvo!");
        std::cout << "make_nrvo counts: ctors=" << g_ctors << " copies=" << g_copies << " moves=" << g_moves << '\n';
        // 理想：1 ctor, 0 copy, 0 move（NRVO）
        // 若无 NRVO：1 ctor + 1 move（隐式移动），仍不应拷贝
        assert(g_copies == 0);
        assert(g_ctors >= 1);
    }

    std::cout << "=== 进阶：分支返回；与 prvalue 对比 ===\n";
    {
        g_ctors = g_copies = g_moves = 0;
        Noisy y = make_branch(true);
        assert(y.id == "A" || y.id == "moved" || !y.id.empty());
        // 分支 NRVO 更难；至少不应依赖「零移动」
        std::cout << "make_branch: ctors=" << g_ctors << " moves=" << g_moves << " copies=" << g_copies
                  << " id=" << y.id << '\n';
        assert(g_copies == 0);  // 隐式移动优先于拷贝
    }

    {
        g_ctors = g_copies = g_moves = 0;
        Noisy z = make_prvalue();
        assert(z.id == "prvalue");
        assert(g_copies == 0);
        std::cout << "prvalue RVO: ctors=" << g_ctors << " moves=" << g_moves << '\n';
    }

    std::cout << "=== 专家：优先级 NRVO → 移动 → 拷贝 ===\n";
    // · NRVO 不在 C++17「保证拷贝消除」范围内（保证的是 prvalue）
    // · 关闭优化 / 复杂控制流时可能不做 NRVO，但 return 局部仍隐式移动
    // · 不要 return std::move(local) 来「帮助」——会阻止 NRVO（见 never_move_return_of_local）
    // · 调试：godbolt + -fno-elide-constructors 观察额外移动

    std::cout << "=== nrvo: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/nrvo", run>;

}  // namespace
