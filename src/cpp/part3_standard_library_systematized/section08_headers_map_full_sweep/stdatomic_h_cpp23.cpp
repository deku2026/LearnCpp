// Topic    : <stdatomic.h>（C++23 C 兼容原子头；无 <cstdatomic>）
// Doc      : 第3部分-标准库系统化.md · headers map C-compat 陷阱
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : stdatomic_h_cpp23
// Topic id : part3/section08/stdatomic_h_cpp23
// Refs     : https://en.cppreference.com/w/cpp/header/stdatomic.h
//            P0943

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>

#if defined(__has_include)
#if __has_include(<stdatomic.h>)
#include <stdatomic.h>
#define LEARN_HAS_STDATOMIC_H 1
#endif
#endif
#ifndef LEARN_HAS_STDATOMIC_H
#define LEARN_HAS_STDATOMIC_H 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [stdatomic_h_cpp23] ===\n";
    std::cout << "C++23 adds <stdatomic.h> for C/_Atomic interop\n";
    std::cout << "there is NO <cstdatomic> counterpart\n";
    std::cout << "prefer <atomic> + std::atomic in pure C++ code\n";

    // 始终可探测的 C++ 原子
    std::atomic<int> cxx{0};
    cxx.store(1, std::memory_order_relaxed);
    assert(cxx.load(std::memory_order_relaxed) == 1);
    assert(cxx.fetch_add(2) == 1);
    assert(cxx.load() == 3);
    std::cout << "std::atomic<int> load=" << cxx.load() << '\n';

#if LEARN_HAS_STDATOMIC_H
    // 在 C++ 中 atomic_int 通常是 std::atomic_int 的别名
    atomic_int c_atom{0};
    c_atom.store(7);
    assert(c_atom.load() == 7);
    std::cout << "atomic_int from <stdatomic.h> load=" << c_atom.load() << '\n';
    std::cout << "<stdatomic.h> included successfully\n";
#else
    std::cout << "<stdatomic.h> not available; documented C++23 addition only\n";
    // 纯 C++ 路径仍保证可运行断言
    assert(std::atomic_int{5}.load() == 5);
#endif

    // 明确写出易混项: 文档 headers map 版本陷阱
    const bool stdbit_is_cpp26 = true;
    const bool stdckdint_is_cpp26 = true;
    assert(stdbit_is_cpp26 && stdckdint_is_cpp26);
    std::cout << "NOT C++23: <stdbit.h> / <stdckdint.h> are C++26\n";
    std::cout << "prefer <atomic> + std::atomic in pure C++ TUs\n";
    std::cout << "[stdatomic_h_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/stdatomic_h_cpp23", run>;

}  // namespace
