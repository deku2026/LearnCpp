// Topic    : headers map · 类型支持 type support
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : type_support_headers
// Topic id : part3/section08/type_support_headers
// Headers  : <type_traits> <typeinfo> <concepts> <compare>
//            <cstddef> <limits>

#include "learn/topic_registry.hpp"

#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <limits>
#include <type_traits>
#include <typeinfo>

namespace {

struct S {
    int x;
    auto operator<=>(const S&) const = default;
};

template <std::integral T>
T twice(T v) {
    return static_cast<T>(v * 2);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [type_support_headers] map probe ===\n";
    std::cout << "<type_traits> is_integral_v<int>=" << std::is_integral_v<int> << '\n';
    std::cout << "<typeinfo>    typeid(int).name()=" << typeid(int).name() << '\n';
    std::cout << "<concepts>    std::integral constrained twice()\n";
    std::cout << "<compare>     operator<=> / ordering\n";
    std::cout << "<cstddef>     size_t / ptrdiff_t / byte / nullptr_t\n";
    std::cout << "<limits>      numeric_limits\n";

    static_assert(std::is_same_v<std::size_t, decltype(sizeof(0))>);
    assert(twice(21) == 42);

    S a{1}, b{2};
    assert((a <=> b) < 0);
    std::cout << "S{1} <=> S{2} is less\n";

    assert(std::numeric_limits<int>::is_integer);
    std::cout << "int max=" << std::numeric_limits<int>::max() << '\n';

    std::byte by{static_cast<unsigned char>(0xFF)};
    assert(std::to_integer<unsigned>(by) == 0xFFu);
    std::cout << "std::byte size=" << sizeof(std::byte) << " value=" << std::to_integer<unsigned>(by) << '\n';

#if defined(__cpp_lib_is_scoped_enum)
    enum class E { A };
    std::cout << "is_scoped_enum_v<E>=" << std::is_scoped_enum_v<E> << " macro=" << __cpp_lib_is_scoped_enum << '\n';
#endif

    std::cout << "[type_support_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/type_support_headers", run>;

}  // namespace
