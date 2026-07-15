// Topic    : 转发引用（万能引用）vs 右值引用
// Doc      : 第2部分-阶段5 · 步骤 6.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : forwarding_reference_universal
// Topic id : part2/stage05/section02/forwarding_reference_universal
// Refs     : https://en.cppreference.com/w/cpp/language/reference#Forwarding_references
//            Effective Modern C++ Item 24

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// 右值引用：T 不在「vector<T>&&」处被推导为引用形式 → 只收右值
template <class T>
void only_rvalue_vector(std::vector<T>&& v) {
    std::cout << "  only_rvalue_vector size=" << v.size() << '\n';
}

// 转发引用：形如 T&& 且 T 正在被推导 → 可绑左值与右值
template <class T>
void fwd_ref(T&& x) {
    using Decayed = std::decay_t<T>;
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "  fwd_ref: deduced T is lvalue-ref, arg stays lvalue-ish\n";
    } else {
        std::cout << "  fwd_ref: deduced T is non-ref (rvalue arg)\n";
    }
    (void)x;
    (void)sizeof(Decayed);
}

// 展示推导结果
template <class T>
std::string describe_fwd(T&& /*x*/) {
    if constexpr (std::is_same_v<T, int&>) {
        return "T=int& (lvalue int)";
    } else if constexpr (std::is_same_v<T, const int&>) {
        return "T=const int&";
    } else if constexpr (std::is_same_v<T, int>) {
        return "T=int (rvalue int)";
    } else if constexpr (std::is_same_v<T, std::string&>) {
        return "T=string&";
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "T=string (rvalue)";
    } else {
        return "T=other";
    }
}

// auto&& 也是转发引用（变量声明 / 泛型 lambda）
template <class F>
void call_with_auto_fwd(F&& /*f*/) {
    // F&& 在函数模板里是转发引用
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [forwarding_reference_universal] 入门：形态 ===\n";
    // 转发引用条件：
    // 1) 函数模板参数类型为 T&&（T 是模板参数）
    // 2) T 在此次调用中被推导（不是指定的）
    // auto&& 局部变量 / 泛型 lambda 的 auto&& 同理

    int n = 1;
    std::cout << "lvalue int: " << describe_fwd(n) << '\n';
    std::cout << "rvalue int: " << describe_fwd(2) << '\n';
    const int cn = 3;
    std::cout << "const lvalue: " << describe_fwd(cn) << '\n';

    std::string s = "hi";
    std::cout << "lvalue string: " << describe_fwd(s) << '\n';
    std::cout << "rvalue string: " << describe_fwd(std::string{"bye"}) << '\n';

    std::cout << "=== 进阶：对比只收右值的 vector&& ===\n";
    {
        std::vector<int> v{1, 2, 3};
        // only_rvalue_vector(v);  // 错误：不能把左值绑到 vector<int>&&
        only_rvalue_vector(std::move(v));
        only_rvalue_vector(std::vector<int>{4, 5});

        fwd_ref(v);                    // OK：转发引用
        fwd_ref(std::vector<int>{9});  // OK
    }

    std::cout << "=== 进阶：auto&& / 范围 for ===\n";
    {
        auto&& r1 = n;   // int&
        auto&& r2 = 10;  // int&&
        static_assert(std::is_same_v<decltype(r1), int&>);
        static_assert(std::is_same_v<decltype(r2), int&&>);
        assert(r1 == 1 && r2 == 10);

        std::vector<std::string> words{"a", "b"};
        for (auto&& w : words) {  // 避免拷贝；可改元素
            w.push_back('!');
        }
        assert(words[0] == "a!" && words[1] == "b!");
        std::cout << "range-for auto&& mutates in place\n";
    }

    std::cout << "=== 专家：不是转发引用的 T&& ===\n";
    // · void f(std::vector<T>&&);  —— T 被 vector 包裹，不是转发引用
    // · void f(const T&&);         —— 多了 const，不是转发引用
    // · template<class T> void f(std::remove_reference_t<T>&&); —— 形式不是 T&&
    // · 结构体成员 T&& m;          —— 与推导无关

    auto generic = [](auto&& x) {
        // 泛型 lambda 的 auto&& 是转发引用
        using X = std::decay_t<decltype(x)>;
        (void)sizeof(X);
    };
    generic(n);
    generic(std::string{"tmp"});
    call_with_auto_fwd(generic);

    std::cout << "=== forwarding_reference_universal: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/forwarding_reference_universal", run>;

}  // namespace
