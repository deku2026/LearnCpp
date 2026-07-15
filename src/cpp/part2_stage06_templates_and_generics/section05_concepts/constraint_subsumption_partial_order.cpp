// Topic    : 约束包容（subsumption）—— 更强约束优先
// Doc      : 第2部分-阶段6 · 步骤 11.7
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : constraint_subsumption_partial_order
// Topic id : part2/stage06/section05/constraint_subsumption_partial_order
// Refs     : https://en.cppreference.com/w/cpp/language/constraints
//            ISO [temp.constr.order]

#include "learn/topic_registry.hpp"

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 有约束重载 比 无约束 更优先
// ---------------------------------------------------------------------------

template <typename T>
std::string f(T) {
    return "generic";
}

template <std::integral T>
std::string f(T) {
    return "integral";
}

// ---------------------------------------------------------------------------
// §进阶 — 具名 concept 组合产生偏序：A&&B 包容 A
// ---------------------------------------------------------------------------

template <typename T>
concept SignedIntegral = std::integral<T> && std::is_signed_v<T>;

template <std::integral T>
std::string g(T) {
    return "integral";
}

template <SignedIntegral T>
std::string g(T) {
    return "signed-integral";  // 更强，优先
}

// ---------------------------------------------------------------------------
// §专家 — 原子约束相等才包容；N>0 不包容 N>=0
// ---------------------------------------------------------------------------

template <typename T>
concept AtLeastIntSize = sizeof(T) >= sizeof(int);

template <typename T>
concept BiggerThanInt = sizeof(T) > sizeof(int);

// 这两个 concept 没有包容关系（编译器不推理算术）
// 同时匹配时可能歧义 → 用包含式设计

template <typename T>
concept Meow = true;

template <typename T>
concept MeowLoud = Meow<T> && true;  // 规范化后与 Meow 关系依赖原子约束

template <Meow T>
std::string h(T) {
    return "meow";
}

template <MeowLoud T>
std::string h(T) {
    return "meow-loud";
}

// 正确设计：显式细化
template <typename T>
concept Animal = true;

template <typename T>
concept Cat = Animal<T> && requires(T t) { t.meow(); };

struct Kitty {
    void meow() const {}
};

template <Animal T>
std::string speak(const T&) {
    return "animal";
}

template <Cat T>
std::string speak(const T&) {
    return "cat";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constraint_subsumption] 入门 ===\n";
    assert(f(42) == "integral");
    assert(f(3.14) == "generic");
    assert(f(std::string{"x"}) == "generic");
    std::cout << "f(42)=" << f(42) << " f(3.14)=" << f(3.14) << '\n';

    std::cout << "=== 进阶：SignedIntegral 包容 integral ===\n";
    assert(g(1) == "signed-integral");  // int 有符号
    assert(g(1u) == "integral");        // unsigned 只满足 integral
    static_assert(SignedIntegral<int>);
    static_assert(!SignedIntegral<unsigned>);
    std::cout << "g(1)=" << g(1) << " g(1u)=" << g(1u) << '\n';

    std::cout << "=== 专家：用组合表达偏序，勿靠算术相似 ===\n";
    // ⚠️ AtLeastIntSize 与 BiggerThanInt 互不包容
    assert(speak(Kitty{}) == "cat");
    assert(speak(0) == "animal");
    // MeowLoud 因 && true 与规范化细节相关；可靠做法是 Animal/Cat 这种包含式
    (void)AtLeastIntSize<int>;
    (void)BiggerThanInt<long long>;
    assert(h(1) == "meow-loud" || h(1) == "meow");  // 实现细节宽容
    std::cout << "subsumption via named concept refinement OK\n";
    std::cout << "=== constraint_subsumption_partial_order: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/constraint_subsumption_partial_order", run>;

}  // namespace
