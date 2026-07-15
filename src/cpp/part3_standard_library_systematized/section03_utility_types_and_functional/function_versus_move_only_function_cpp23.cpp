// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.2 function / move_only_function
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : function_versus_move_only_function_cpp23
// Topic id : part3/section03/function_versus_move_only_function_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/functional/function
//   https://en.cppreference.com/w/cpp/utility/functional/move_only_function
//   Feature-test: __cpp_lib_move_only_function >= 202110L
//
// 要点: function 可拷贝类型擦除; move_only_function 可存 unique_ptr 捕获。

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <memory>
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

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/function_versus_move_only_function_cpp23 ===\n";

    std::function<int(int, int)> f = add;
    assert(f(3, 4) == 7);

    // 可拷贝: 小对象可能 SBO; 大对象堆分配
    std::function<int(int)> g = [n = 10](int x) { return x + n; };
    assert(g(5) == 15);
    auto g2 = g;  // copy
    assert(g2(1) == 11);

    // 空 function 调用抛 bad_function_call
    std::function<void()> empty;
    assert(!empty);
    [[maybe_unused]] bool threw = false;
    try {
        empty();
    } catch (const std::bad_function_call&) {
        threw = true;
    }
    assert(threw);

    std::cout << "[intro] std::function copyable type erasure ok\n";

#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    // 只移动: 可包装不可拷贝的可调用
    auto up = std::make_unique<std::string>("owned");
    std::move_only_function<int()> mof = [p = std::move(up)]() { return static_cast<int>(p->size()); };
    assert(mof() == 5);
    // auto mof2 = mof; // 错误: 不可拷贝
    auto mof2 = std::move(mof);
    assert(mof2() == 5);
    std::cout << "[advanced] move_only_function holds unique_ptr capture\n";
#else
    // 降级: 直接用不可拷贝 lambda, 不放进 function
    auto up = std::make_unique<std::string>("owned");
    auto lam = [p = std::move(up)]() { return static_cast<int>(p->size()); };
    assert(lam() == 5);
    std::cout << "[advanced] move_only_function unavailable; used move-only "
                 "lambda directly\n";
#endif

    std::cout << "[expert] prefer function only when you need copyable "
                 "type-erased callables\n";
    std::cout << "function_versus_move_only_function_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/function_versus_move_only_function_cpp23", run>;

}  // namespace
