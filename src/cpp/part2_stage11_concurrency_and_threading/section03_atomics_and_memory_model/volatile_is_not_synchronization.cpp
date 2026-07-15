// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 9.2 验收点
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : volatile_is_not_synchronization
// Topic id : part2/stage11/section03/volatile_is_not_synchronization
// Refs     : https://en.cppreference.com/w/cpp/language/cv
//            https://en.cppreference.com/w/cpp/atomic/atomic
//
// Acceptance: explain why C++ volatile is NOT thread synchronization.
// volatile ≈ hardware/MMIO/signal; concurrency ≈ atomic or mutex.
// (Java volatile ≈ roughly C++ atomic seq_cst — different languages!)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [volatile ≠ sync] what volatile actually guarantees ===\n";
    std::cout << "  · actual memory access (not optimized into a register forever)\n"
                 "  · no reordering of volatile accesses among themselves\n"
                 "  It does NOT provide: atomicity, acquire/release, happens-before.\n";

    std::cout << "=== WRONG pattern (do not use for threads) ===\n";
    std::cout << "  volatile bool ready = false;  // NOT a synchronization tool\n"
                 "  volatile int data = 0;\n"
                 "  // thread A: data = 42; ready = true;\n"
                 "  // thread B: while (!ready) {} use(data);\n"
                 "  Still a data race on both variables; visibility of data\n"
                 "  is not guaranteed; ++ on volatile int is still non-atomic.\n";

    std::cout << "=== CORRECT: atomic flag + non-atomic payload ===\n";
    {
        int data = 0;
        std::atomic<bool> ready{false};

        std::jthread producer([&] {
            data = 42;
            ready.store(true, std::memory_order_release);
        });
        std::jthread consumer([&] {
            while (!ready.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            assert(data == 42);
            std::cout << "  atomic publish: data=" << data << '\n';
        });
    }

    std::cout << "=== CORRECT: atomic payload when the value itself is shared ===\n";
    {
        std::atomic<int> counter{0};
        {
            std::jthread t1([&] {
                for (int i = 0; i < 1000; ++i) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
            std::jthread t2([&] {
                for (int i = 0; i < 1000; ++i) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        assert(counter.load() == 2000);
        std::cout << "  atomic RMW counter=" << counter.load() << '\n';
    }

    std::cout << "one-liner: thread sync → std::atomic or mutex; never volatile\n";
    std::cout << "[volatile_is_not_synchronization] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/volatile_is_not_synchronization", run>;

}  // namespace
