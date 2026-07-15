// Topic    : void_t 检测惯用法 —— 探测成员/嵌套类型/合法表达式
// Doc      : 第2部分-阶段6 · 步骤 9.4
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : void_t
// Topic id : part2/stage06/section04/void_t
// Refs     : https://en.cppreference.com/w/cpp/types/void_t
//            Walter Brown CppCon 2016 detection idiom

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — void_t 把「合法类型包」映射到 void
// ---------------------------------------------------------------------------

// 标准：template<class...> using void_t = void;

template <typename T, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

// ---------------------------------------------------------------------------
// §进阶 — 探测成员函数 size()；探测可加
// ---------------------------------------------------------------------------

template <typename T, typename = void>
struct has_size_method : std::false_type {};

template <typename T>
struct has_size_method<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template <typename T, typename = void>
struct is_addable : std::false_type {};

template <typename T>
struct is_addable<T, std::void_t<decltype(std::declval<T>() + std::declval<T>())>> : std::true_type {};

// ---------------------------------------------------------------------------
// §专家 — 完整 detection idiom；探测运算符 <<
// ---------------------------------------------------------------------------

template <typename, template <typename...> class, typename = void>
struct detector : std::false_type {};

template <typename Default, template <typename...> class Op>
struct detector<Default, Op, std::void_t<Op<Default>>> : std::true_type {
    using type = Op<Default>;
};

template <typename T>
using size_op = decltype(std::declval<T>().size());

template <typename T>
using detected_size = detector<T, size_op>;

template <typename T, typename = void>
struct is_ostreamable : std::false_type {};

template <typename T>
struct is_ostreamable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>
    : std::true_type {};

struct WithVT {
    using value_type = int;
};
struct WithoutVT {};
struct NoSize {};

struct StreamMe {
    int v = 0;
    friend std::ostream& operator<<(std::ostream& os, StreamMe s) { return os << s.v; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [void_t] 入门：嵌套类型探测 ===\n";
    static_assert(has_value_type<WithVT>::value);
    static_assert(!has_value_type<WithoutVT>::value);
    static_assert(has_value_type<std::vector<int>>::value);
    std::cout << "has_value_type vector=" << has_value_type<std::vector<int>>::value << '\n';

    std::cout << "=== 进阶：成员与运算符探测 ===\n";
    static_assert(has_size_method<std::vector<int>>::value);
    static_assert(has_size_method<std::string>::value);
    static_assert(!has_size_method<NoSize>::value);
    static_assert(is_addable<int>::value);
    static_assert(is_addable<std::string>::value);
    static_assert(!is_addable<WithoutVT>::value);
    std::cout << "size/addable detectors OK\n";

    std::cout << "=== 专家：detector 封装 + ostreamable ===\n";
    static_assert(detected_size<std::string>::value);
    static_assert(!detected_size<int>::value);
    static_assert(is_ostreamable<int>::value);
    static_assert(is_ostreamable<StreamMe>::value);
    static_assert(!is_ostreamable<WithoutVT>::value);
    // 原理：void_t<非法表达式> 替换失败 → 回退主模板 false_type
    std::cout << "=== void_t: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/void_t", run>;

}  // namespace
