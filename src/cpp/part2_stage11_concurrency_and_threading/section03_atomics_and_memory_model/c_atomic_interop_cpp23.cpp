// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 9.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : c_atomic_interop_cpp23
// Topic id : part2/stage11/section03/c_atomic_interop_cpp23
// Refs     : https://en.cppreference.com/w/cpp/header/stdatomic.h
//            P0943 Support C atomics in C++
//            ISO C++23 <stdatomic.h>
//
// C++23 clarifies C/C++ atomic interop via <stdatomic.h> so mixed codebases
// can share atomic objects more portably. Prefer std::atomic in pure C++.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>

// C++23: including <stdatomic.h> in C++ provides C-compatible atomic macros
// / types that interoperate with the C++ atomics library.
#if __has_include(<stdatomic.h>)
#include <stdatomic.h>
#define LEARNCPP_HAS_STDATOMIC_H 1
#else
#define LEARNCPP_HAS_STDATOMIC_H 0
#endif

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [C++23] C atomic interop overview ===\n";
    std::cout << "  C11 _Atomic(T) / <stdatomic.h> and C++ std::atomic<T>\n"
                 "  are intended to be layout-compatible for shared headers.\n"
                 "  C++23 improves the <stdatomic.h> story for C++ TUs.\n";

    std::cout << "=== pure C++ side (always available) ===\n";
    {
        std::atomic<int> cpp_counter{0};
        {
            std::jthread t1([&] {
                for (int i = 0; i < 500; ++i) {
                    cpp_counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
            std::jthread t2([&] {
                for (int i = 0; i < 500; ++i) {
                    cpp_counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        assert(cpp_counter.load() == 1000);
        std::cout << "  std::atomic counter=" << cpp_counter.load() << '\n';
    }

#if LEARNCPP_HAS_STDATOMIC_H
    std::cout << "=== <stdatomic.h> visible from C++ ===\n";
    {
        // atomic_int is the C typedef (also provided for C++ via the header).
        atomic_int c_style{};
        atomic_store(&c_style, 0);
        {
            std::jthread t1([&] {
                for (int i = 0; i < 500; ++i) {
                    atomic_fetch_add_explicit(&c_style, 1, memory_order_relaxed);
                }
            });
            std::jthread t2([&] {
                for (int i = 0; i < 500; ++i) {
                    atomic_fetch_add_explicit(&c_style, 1, memory_order_relaxed);
                }
            });
        }
        assert(atomic_load(&c_style) == 1000);
        std::cout << "  atomic_int via stdatomic.h=" << atomic_load(&c_style) << '\n';
    }
#else
    std::cout << "=== <stdatomic.h> not available on this toolchain header set ===\n";
    std::cout << "  fallback: use std::atomic only (shown above)\n";
#endif

    std::cout << "guidance: new C++ code → std::atomic; shared C headers →\n"
                 "  C++23 <stdatomic.h> / compatible _Atomic objects.\n";
    std::cout << "[c_atomic_interop_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/c_atomic_interop_cpp23", run>;

}  // namespace
