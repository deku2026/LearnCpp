// Topic    : constexpr unique_ptr（C++23 编译期智能指针）
// Doc      : 第3部分-标准库系统化.md · 库 6.1
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part3/section06/constexpr_unique_ptr_cpp23
// Refs     : https://en.cppreference.com/w/cpp/memory/unique_ptr
//            P2273 / P2447 相关；__cpp_lib_constexpr_memory / unique_ptr

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
constexpr int demo_constexpr_unique_ptr() {
    std::unique_ptr<int> p = std::make_unique<int>(21);
    auto q = std::move(p);
    if (!q) {
        return -1;
    }
    *q += 21;
    return *q;
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constexpr_unique_ptr_cpp23] ===\n";

#if defined(__cpp_lib_constexpr_memory)
    std::cout << "__cpp_lib_constexpr_memory=" << __cpp_lib_constexpr_memory << '\n';
#endif

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    constexpr int v = demo_constexpr_unique_ptr();
    static_assert(v == 42);
    std::cout << "constexpr unique_ptr result=" << v << '\n';
#else
    // 运行期路径仍验证 unique_ptr 语义
    auto p = std::make_unique<int>(21);
    auto q = std::move(p);
    assert(!p && q);
    *q += 21;
    assert(*q == 42);
    std::cout << "runtime unique_ptr result=" << *q << " (constexpr unique_ptr not available)\n";
#endif

    // C++23 意义：常量求值里也能做 RAII 式堆分配（实现定义 new 在 consteval 中）
    std::cout << "note: enables RAII allocation patterns inside constant evaluation\n";
    std::cout << "[constexpr_unique_ptr_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/constexpr_unique_ptr_cpp23", run>;

}  // namespace
