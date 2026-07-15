// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.2 bind_front/back / invoke
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : bind_front_back_and_invoke_invoke_r_cpp23
// Topic id : part3/section03/bind_front_back_and_invoke_invoke_r_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/functional/bind_front
//   https://en.cppreference.com/w/cpp/utility/functional/bind_back
//   https://en.cppreference.com/w/cpp/utility/functional/invoke
//   Feature-test: __cpp_lib_bind_back / __cpp_lib_invoke_r

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int add(int a, int b) {
    return a + b;
}

struct Widget {
    int value{0};
    int get() const { return value; }
    void set(int v) { value = v; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/bind_front_back_and_invoke_invoke_r_cpp23 ===\n";

    auto add5 = std::bind_front(add, 5);
    assert(add5(10) == 15);
    std::cout << "[intro] bind_front(add,5)(10)=" << add5(10) << '\n';

#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    auto add_tail = std::bind_back(add, 7);
    assert(add_tail(3) == 10);
    std::cout << "[advanced] bind_back(add,7)(3)=" << add_tail(3) << '\n';
#else
    auto add_tail = [](int a) { return add(a, 7); };
    assert(add_tail(3) == 10);
    std::cout << "[advanced] bind_back unavailable; lambda stand-in\n";
#endif

    // invoke: 统一函数/成员函数/成员指针/函数对象
    assert(std::invoke(add, 1, 2) == 3);
    Widget w{42};
    assert(std::invoke(&Widget::get, w) == 42);
    std::invoke(&Widget::set, w, 99);
    assert(w.value == 99);
    assert(std::invoke(&Widget::value, w) == 99);

#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    // invoke_r: 指定返回类型(可做隐式转换)
    const long r = std::invoke_r<long>(add, 2, 3);
    assert(r == 5L);
    std::cout << "[advanced] invoke_r<long>=" << r << '\n';
#else
    const long r = static_cast<long>(std::invoke(add, 2, 3));
    assert(r == 5L);
    std::cout << "[advanced] invoke_r unavailable; cast after invoke\n";
#endif

    // 新代码: 优先 lambda / bind_front/back, 少用旧 std::bind 占位符
    std::cout << "[expert] avoid legacy std::bind placeholders when possible\n";
    std::cout << "bind_front_back_and_invoke_invoke_r_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/bind_front_back_and_invoke_invoke_r_cpp23", run>;

}  // namespace
