// Topic     : C++17 prvalue 与临时量物化 / 保证拷贝消除
// Doc       : 第2部分-阶段2 · 步骤 5.4
// cppreference: https://en.cppreference.com/cpp/language/value_category
//               https://en.cppreference.com/cpp/language/copy_elision
//               https://en.cppreference.com/cpp/language/implicit_conversion
//                 (Temporary materialization)
//
// 要点: C++17 起 prvalue 是“初始化配方”，不必然先有临时对象；
//       同类型初始化保证不调用拷贝/移动；需要对象身份时才物化。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

struct NoCopyMove {
    int n;
    explicit NoCopyMove(int v) : n{v} { std::cout << "[ctor] NoCopyMove(" << n << ")\n"; }
    NoCopyMove(const NoCopyMove&) = delete;
    NoCopyMove(NoCopyMove&&) = delete;
    NoCopyMove& operator=(const NoCopyMove&) = delete;
    NoCopyMove& operator=(NoCopyMove&&) = delete;
};

NoCopyMove make_ncm(int v) {
    // 返回 prvalue：C++17 直接在调用方存储构造，不需要移动构造
    return NoCopyMove{v};
}

NoCopyMove make_with_local(int v) {
    // 具名局部返回：依赖 NRVO（可选优化）或隐式移动；
    // 因移动已 delete，此处必须靠 NRVO 才能编译——实现可能不同。
    // 为可移植教学，改用纯 prvalue 路径（上面 make_ncm）。
    return NoCopyMove{v};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [prvalue_materialization_cpp17] ===\n";

    // -------------------------------------------------------------------------
    // 入门：保证的拷贝消除
    // -------------------------------------------------------------------------
    // C++14：T x = make(); 可能需要可访问的拷贝/移动（即使被 elide）
    // C++17：同类型 prvalue 初始化 → 不要求拷贝/移动，且不调用它们
    [[maybe_unused]] NoCopyMove x = make_ncm(1);
    assert(x.n == 1);

    NoCopyMove y = NoCopyMove{2};  // 同样是 prvalue 初始化
    assert(y.n == 2);
    std::cout << "[intro] guaranteed elision: deleted move/copy still OK\n";

    // -------------------------------------------------------------------------
    // 进阶：临时量物化 temporary materialization
    // -------------------------------------------------------------------------
    // 当 prvalue 需要作为 glvalue 使用（绑引用、. 成员、取地址等）时，
    // 才物化出临时对象（结果是 xvalue）。
    [[maybe_unused]] const NoCopyMove& ref = make_ncm(3);  // 物化临时，const& 延长寿命
    assert(ref.n == 3);

    // 成员访问也触发物化
    [[maybe_unused]] int n = make_ncm(4).n;
    assert(n == 4);

    // 纯初始化目标对象：不单独物化再移动
    [[maybe_unused]] NoCopyMove z{make_ncm(5)};
    assert(z.n == 5);
    std::cout << "[advanced] materialization when a real object identity is required\n";

    // -------------------------------------------------------------------------
    // 专家：日常类型 + 「保证消除」的更精确说法
    // -------------------------------------------------------------------------
    // cppreference / MS 博文：C++17 与其说是“强制 elide 一次已存在的拷贝”，
    // 不如说是 **deferred temporary materialization**：
    // prvalue 在需要 glvalue 身份之前根本不对应独立临时对象，因此无拷/移可 elide。
    std::string s = std::string{"hello"};  // 直接在 s 存储构造
    assert(s == "hello");

    auto factory = [](const char* p) { return std::string{p}; };
    std::string t = factory("world");
    assert(t == "world");

    // 必须物化：绑引用 / 成员访问 → 先有临时对象（xvalue），const& 可延长寿命
    std::string&& rr = factory("xvalue path");
    assert(rr == "xvalue path");
    const std::string& crext = factory("extend");
    assert(crext == "extend");

    // make_with_local 与 make_ncm 在“返回 prvalue 构造表达式”时等价安全
    [[maybe_unused]] NoCopyMove w = make_with_local(6);
    assert(w.n == 6);

    // 对照：若只有具名局部再 return，则依赖 NRVO/隐式移动（移动已 delete 时不可移植）
    // 本文件刻意用 prvalue 返回路径保证 C++17 下可编译。

    std::cout << "[expert] C++17: prvalue = initializing recipe; materialize only when needed\n";
    std::cout << "=== prvalue_materialization_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/prvalue_materialization_cpp17", run>;

}  // namespace
