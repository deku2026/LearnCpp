// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.2 reference_wrapper / forward_like
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : reference_wrapper_and_forward_like_cpp23
// Topic id : part3/section03/reference_wrapper_and_forward_like_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
//   https://en.cppreference.com/w/cpp/utility/forward_like
//   Feature-test: __cpp_lib_forward_like >= 202207L
//
// 要点: ref/cref 让容器/bind 存引用; forward_like 按另一对象值类别转发。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
template <class T, class U>
decltype(auto) like_fwd(T&& /*probe*/, U&& u) {
    return std::forward_like<T>(u);
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/reference_wrapper_and_forward_like_cpp23 ===\n";

    int x = 10;
    std::reference_wrapper<int> rw = std::ref(x);
    rw.get() = 20;
    assert(x == 20);

    const int y = 5;
    [[maybe_unused]] std::reference_wrapper<const int> cr = std::cref(y);
    assert(cr.get() == 5);

    // 容器默认存值; ref 可存“引用语义”
    int a = 1, b = 2;
    std::vector<std::reference_wrapper<int>> refs{std::ref(a), std::ref(b)};
    refs[0].get() = 100;
    assert(a == 100);

    // pair 里显式存 reference_wrapper(不要用会误导的裸引用模板参数)
    std::pair<std::reference_wrapper<int>, int> pr{std::ref(x), 1};
    pr.first.get() = 30;
    assert(x == 30);

    std::cout << "[intro] ref/cref + vector of reference_wrapper ok\n";

#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    std::string s = "data";
    // 用左值 probe → 转发成左值引用
    decltype(auto) l = like_fwd(s, s);
    static_assert(std::is_lvalue_reference_v<decltype(l)>);

    // 用右值 probe → 转发成右值引用
    decltype(auto) r = like_fwd(std::move(s), s);
    static_assert(std::is_rvalue_reference_v<decltype(r)>);
    std::cout << "[advanced] forward_like preserves value category of probe\n";
#else
    std::cout << "[advanced] forward_like unavailable on this STL\n";
#endif

    std::cout << "[expert] forward_like pairs well with deducing this helpers\n";
    std::cout << "reference_wrapper_and_forward_like_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/reference_wrapper_and_forward_like_cpp23", run>;

}  // namespace
