// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E2 SFINAE 深水)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : detection_idiom
// Topic id : part6/e/section02/detection_idiom
//
// 要点: 标准 detection idiom = is_detected / detected_t / detected_or
//       (实验性 <experimental/type_traits> 思路; 这里手写等价物)。
// 参考: N4502; Walter Brown; cppreference is_detected

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// --- 手写 is_detected ---
struct nonesuch {
    nonesuch() = delete;
    ~nonesuch() = delete;
    nonesuch(const nonesuch&) = delete;
    void operator=(const nonesuch&) = delete;
};

template <typename Default, typename AlwaysVoid, template <typename...> class Op, typename... Args>
struct detector {
    using value_t = std::false_type;
    using type = Default;
};

template <typename Default, template <typename...> class Op, typename... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
    using value_t = std::true_type;
    using type = Op<Args...>;
};

template <template <typename...> class Op, typename... Args>
using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

template <template <typename...> class Op, typename... Args>
using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

template <typename Default, template <typename...> class Op, typename... Args>
using detected_or = detector<Default, void, Op, Args...>;

template <typename Default, template <typename...> class Op, typename... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

// 操作别名
template <typename T>
using value_type_t = typename T::value_type;

template <typename T>
using begin_t = decltype(std::declval<T&>().begin());

template <typename T>
using preinc_t = decltype(++std::declval<T&>());

// 业务: 若有 value_type 用它, 否则用 Default
template <typename T>
using value_or_int = detected_or_t<int, value_type_t, T>;

// 若可 begin 则取 *begin 类型
template <typename T>
using element_t = std::remove_reference_t<decltype(*std::declval<begin_t<T>>())>;

struct Plain {
    int x;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E2 detection idiom ===\n";

    static_assert(is_detected<value_type_t, std::vector<int>>::value);
    static_assert(!is_detected<value_type_t, Plain>::value);
    static_assert(is_detected<begin_t, std::vector<int>>::value);
    static_assert(!is_detected<begin_t, int>::value);

    static_assert(std::is_same_v<detected_t<value_type_t, std::vector<char>>, char>);
    // detected_t 失败时是 nonesuch —— 不要在失败路径求值 detected_t

    static_assert(std::is_same_v<value_or_int<std::vector<double>>, double>);
    static_assert(std::is_same_v<value_or_int<Plain>, int>);

    static_assert(std::is_same_v<element_t<std::vector<std::string>>, std::string>);

    // 运行时 sanity
    using VT = detected_or_t<void, value_type_t, std::vector<int>>;
    static_assert(std::is_same_v<VT, int>);
    assert((is_detected<preinc_t, int*>::value));

    std::cout << "  is_detected / detected_or mirror library traits\n";
    std::cout << "  C++20: prefer requires { typename T::value_type; } concepts\n";
    std::cout << "detection_idiom: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/detection_idiom", run>;

}  // namespace
