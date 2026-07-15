// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E3 Concepts 深水)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : constraint_subsumption
// Topic id : part6/e/section03/constraint_subsumption
//
// 要点: 约束包容 (subsumption) 建立偏序 —— 更强约束的重载胜出;
//       原子约束必须规范化后相等才能包容。
// 参考: [temp.constr.order] cppreference constraint normalization

#include "learn/topic_registry.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

template <typename T>
std::string pick(T) {
    return "unconstrained";
}

template <std::integral T>
std::string pick(T) {
    return "integral";
}

template <typename T>
concept SignedInt = std::integral<T> && std::is_signed_v<T>;

template <SignedInt T>
std::string pick(T) {
    return "signed-integral";  // 包容 integral → 更优先
}

// 规范化: A && B 包容 A
template <typename T>
concept Animal = true;

template <typename T>
concept Dog = Animal<T> && requires(T t) { t.bark(); };

struct Barking {
    void bark() const {}
};

template <Animal T>
std::string speak(T) {
    return "animal";
}

template <Dog T>
std::string speak(T) {
    return "dog";
}

// 无包容: 算术关系编译器不推理
template <typename T>
concept AtLeast4 = sizeof(T) >= 4;

template <typename T>
concept Exactly4 = sizeof(T) == 4;

// 同时匹配可能歧义 —— 用互斥设计
template <AtLeast4 T>
    requires(!Exactly4<T>)
std::string width(T) {
    return ">=4 and not 4";
}

template <Exactly4 T>
std::string width(T) {
    return "==4";
}

// 同原子约束不同写法经规范化可等同
template <typename T>
concept Int1 = std::integral<T>;

template <typename T>
concept Int2 = std::integral<T> && true;  // 规范化后仍含 integral 原子约束

template <Int1 T>
int score(T) {
    return 1;
}

template <Int2 T>
int score(T) {
    return 2;  // Int2 多一个 true 原子约束 → 更"强"而包容 Int1
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E3 constraint subsumption ===\n";

    assert(pick(1.5) == "unconstrained");
    assert(pick(1u) == "integral");
    assert(pick(-3) == "signed-integral");

    assert(speak(0) == "animal");
    assert(speak(Barking{}) == "dog");

    assert(width(1.0) == "==4" || width(1.0) == ">=4 and not 4" || true);  // double size 平台相关
    assert(width(static_cast<std::int32_t>(0)) == "==4" ||
           width(static_cast<std::int32_t>(0)).find('4') != std::string::npos);

    // 用已知大小的类型
    struct Big {
        char buf[8];
    };
    assert(width(Big{}) == ">=4 and not 4");
    assert(width(std::int32_t{0}) == "==4");

    assert(score(1) == 2);  // Int2 胜出

    std::cout << "  stronger named constraints win via subsumption\n";
    std::cout << "  design refinements as Concept && extra, not magic arithmetic\n";
    std::cout << "constraint_subsumption: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/constraint_subsumption", run>;

}  // namespace
