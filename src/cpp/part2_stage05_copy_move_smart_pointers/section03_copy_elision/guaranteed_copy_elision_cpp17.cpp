// Topic    : C++17 保证的拷贝消除（prvalue 物化，无需可移动）
// Doc      : 第2部分-阶段5 · 步骤 5.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : guaranteed_copy_elision_cpp17
// Topic id : part2/stage05/section03/guaranteed_copy_elision_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/copy_elision
//            https://en.cppreference.com/w/cpp/language/value_category

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <utility>

namespace {

// 删除拷贝与移动：C++17 前几乎无法从函数返回
struct Locked {
    int value;
    explicit Locked(int v) : value(v) { std::cout << "  Locked(" << value << ")\n"; }
    Locked(const Locked&) = delete;
    Locked(Locked&&) = delete;
    Locked& operator=(const Locked&) = delete;
    Locked& operator=(Locked&&) = delete;
    ~Locked() { std::cout << "  ~Locked(" << value << ")\n"; }
};

Locked make_locked(int v) {
    return Locked{v};  // prvalue → 保证在目标位置构造
}

// 只返回 prvalue 的工厂
Locked factory() {
    return Locked{7};
}

// 注意：若写成 return named_locked; 则需要移动/NRVO，
// 在 Locked 删除移动后将无法编译 —— 这正是「保证消除仅覆盖 prvalue」的边界。

// 初始化：T x = T(args) / T x = factory();
struct Guard {
    std::unique_ptr<int> p;
    explicit Guard(int v) : p(std::make_unique<int>(v)) {}
    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    Guard& operator=(const Guard&) = delete;
    Guard& operator=(Guard&&) = delete;
};

Guard make_guard() {
    return Guard{99};  // 不可移动也能返回
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [guaranteed_copy_elision_cpp17] 入门 ===\n";

    Locked a = make_locked(1);
    assert(a.value == 1);

    Locked b = factory();
    assert(b.value == 7);

    // 纯 prvalue 初始化
    Locked c = Locked{3};
    assert(c.value == 3);

    std::cout << "=== 进阶：不可移动类型放入可选场景 ===\n";
    {
        Guard g = make_guard();
        assert(g.p && *g.p == 99);
        std::cout << "Guard via guaranteed elision *p=" << *g.p << '\n';
    }

    std::cout << "=== 进阶：保证范围 vs 非保证 ===\n";
    // 保证（C++17）：
    //   · 用 prvalue 初始化同类型对象：T x = factory();
    //   · return prvalue;  throw prvalue; 等
    // 不保证：
    //   · return named_local;  （NRVO，可选）
    //   · 从函数返回后再绑定到另一类型等需转换的场景
    //
    // 语言层面：prvalue 不再「先造临时再移动」，而是延迟物化到最终对象

    Locked d{5};
    assert(d.value == 5);
    // Locked e = std::move(d); // 错误：移动已删除

    std::cout << "=== 专家：与隐式移动的分工 ===\n";
    // · prvalue 路径：保证消除，连移动构造都可以没有
    // · 具名返回：尽量 NRVO；否则隐式移动（需要移动构造）
    // · 教学：-fno-elide-constructors 对 C++17 prvalue 语义影响因编译器而异

    std::cout << "=== guaranteed_copy_elision_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/guaranteed_copy_elision_cpp17", run>;

}  // namespace
