// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §3.1
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : detection_idiom
// Topic id : part2/stage15/section03/detection_idiom
//
// Detection idiom: compile-time "does this type support X?"
// Motive : branch generic code on presence of members/ops without hard errors.
// Modern : C++20 requires/concepts (preferred); classic void_t specialization.
// Pitfall: detecting syntax ≠ detecting semantics; keep requires expressions tight.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------- classic C++17 void_t detection ----------
template <typename, typename = void>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<const T&>().size())>> : std::true_type {};

template <typename T>
inline constexpr bool has_size_v = has_size<T>::value;

template <typename, typename = void>
struct has_push_back : std::false_type {};

template <typename T>
struct has_push_back<T, std::void_t<decltype(std::declval<T&>().push_back(std::declval<typename T::value_type>()))>>
    : std::true_type {};

// Library-style is_detected sketch
namespace detail {
template <typename AlwaysVoid, template <typename...> class Op, typename... Args>
struct detector : std::false_type {
    using type = void;
};
template <template <typename...> class Op, typename... Args>
struct detector<std::void_t<Op<Args...>>, Op, Args...> : std::true_type {
    using type = Op<Args...>;
};
}  // namespace detail

template <template <typename...> class Op, typename... Args>
using is_detected = detail::detector<void, Op, Args...>;

template <typename T>
using begin_t = decltype(std::begin(std::declval<T&>()));

// ---------- modern C++20 ----------
template <typename T>
concept HasSize = requires(const T& t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept HasPushBack = requires(T& t, typename T::value_type v) { t.push_back(v); };

template <typename T>
void describe(const T& t) {
    if constexpr (HasSize<T>) {
        std::cout << "  has size=" << t.size() << '\n';
    } else {
        std::cout << "  no size()\n";
    }

    if constexpr (has_size_v<T>) {
        // classic trait agrees with concept for these cases
        assert(HasSize<T>);
    }
}

template <typename C>
void maybe_push(C& c, typename C::value_type v) {
    if constexpr (HasPushBack<C>) {
        c.push_back(std::move(v));
        std::cout << "  pushed, size now " << c.size() << '\n';
    } else {
        std::cout << "  cannot push_back\n";
    }
}

struct NoSize {
    int value = 0;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== void_t detection ==\n";
    static_assert(has_size_v<std::vector<int>>);
    static_assert(!has_size_v<int>);
    static_assert(!has_size_v<NoSize>);
    static_assert(is_detected<begin_t, std::vector<int>>::value);
    std::cout << "  has_size<vector>=" << has_size_v<std::vector<int>> << " has_size<int>=" << has_size_v<int> << '\n';

    std::cout << "== concepts + if constexpr ==\n";
    std::vector<int> v{1, 2, 3};
    describe(v);
    describe(NoSize{});
    maybe_push(v, 4);
    assert(v.size() == 4);

    std::string s = "ab";
    maybe_push(s, 'c');
    assert(s == "abc");

    std::cout << "prefer requires/concepts in new code; void_t still everywhere in old libs\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/detection_idiom", run>;

}  // namespace
