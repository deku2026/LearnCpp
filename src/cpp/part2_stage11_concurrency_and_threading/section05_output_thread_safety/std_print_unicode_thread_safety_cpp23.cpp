// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 10.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : std_print_unicode_thread_safety_cpp23
// Topic id : part2/stage11/section05/std_print_unicode_thread_safety_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/print
//            P2093 · ISO [print.fun]
//
// C++23 std::print / println: a single call to the same stream is typically
// not character-interleaved (implementation uses locked stream writes /
// native Unicode APIs). Multiple calls can still interleave — use
// osyncstream or a mutex when multi-line atomicity is required.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#define LEARNCPP_HAS_PRINT 1
#else
#define LEARNCPP_HAS_PRINT 0
#endif

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [print C++23] thread-safety notes ===\n";
#if defined(__cpp_lib_print)
    std::cout << "  __cpp_lib_print=" << __cpp_lib_print << '\n';
#endif

#if LEARNCPP_HAS_PRINT
    std::cout << "=== concurrent println (single-call atomicity) ===\n";
    {
        std::atomic<int> done{0};
        {
            std::vector<std::jthread> pool;
            for (int id = 0; id < 6; ++id) {
                pool.emplace_back([id, &done] {
                    // One call → one contiguous transfer for this message.
                    std::println("worker {} 完成 / done", id);
                    done.fetch_add(1, std::memory_order_relaxed);
                });
            }
        }
        assert(done.load() == 6);
        std::cout << "  six println calls completed\n";
    }

    std::cout << "=== print to stdout with Unicode sample ===\n";
    {
        std::print("  π≈{}  emoji={}  CJK={}\n", 3.14159, "✓", "并发");
    }

    std::cout << "=== multi-call still needs external sync for grouping ===\n";
    {
        std::mutex log_mtx;
        {
            std::vector<std::jthread> pool;
            for (int id = 0; id < 3; ++id) {
                pool.emplace_back([id, &log_mtx] {
                    std::lock_guard lock(log_mtx);
                    std::println("begin {}", id);
                    std::println("end {}", id);
                });
            }
        }
        std::cout << "  mutex groups begin/end pairs\n";
    }
#else
    std::cout << "=== <print> not available; fallback to iostream ===\n";
    {
        std::mutex m;
        std::atomic<int> done{0};
        {
            std::vector<std::jthread> pool;
            for (int id = 0; id < 4; ++id) {
                pool.emplace_back([id, &m, &done] {
                    {
                        std::lock_guard lock(m);
                        std::cout << "worker " << id << " done\n";
                    }
                    done.fetch_add(1, std::memory_order_relaxed);
                });
            }
        }
        assert(done.load() == 4);
    }
#endif

    std::cout << "prefer println for one-liners; osyncstream for multi-insert\n"
                 "iostream chains; mutex when a multi-line transaction matters.\n";
    std::cout << "[std_print_unicode_thread_safety_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section05/std_print_unicode_thread_safety_cpp23", run>;

}  // namespace
