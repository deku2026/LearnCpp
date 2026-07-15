// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 0 / 8.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : happens_before_intro
// Topic id : part2/stage11/section03/happens_before_intro
// Refs     : https://en.cppreference.com/w/cpp/language/memory_model
//            ISO [intro.races]
//
// happens-before: the cross-thread "visible and ordered" relation.
// synchronizes-with (e.g. release→acquire on the same atomic) is one way
// to establish inter-thread happens-before. No HB + concurrent write = race.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [happens-before] single-thread sequenced-before ===\n";
    {
        int a = 1;
        [[maybe_unused]] int b = a + 1;  // a sequenced-before b (same thread)
        assert(b == 2);
        std::cout << "  sequenced-before is the single-thread base of HB\n";
    }

    std::cout << "=== inter-thread HB via mutex (unlock→lock) ===\n";
    {
        std::mutex m;
        int data = 0;

        std::jthread writer([&] {
            std::lock_guard lock(m);
            data = 42;  // unlock will release-publish this write
        });
        writer.join();  // join also contributes HB from thread end to joiner

        {
            std::lock_guard lock(m);
            assert(data == 42);
        }
        std::cout << "  join + mutex: writer’s store happens-before reader\n";
    }

    std::cout << "=== inter-thread HB via release/acquire ===\n";
    {
        int data = 0;
        std::atomic<bool> flag{false};

        std::jthread prod([&] {
            data = 7;
            flag.store(true, std::memory_order_release);  // synchronizes-with ↓
        });
        std::jthread cons([&] {
            while (!flag.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            assert(data == 7);
        });
        std::cout << "  release store synchronizes-with acquire load\n";
    }

    std::cout << "=== recipe for race-free code ===\n";
    std::cout << "  For every pair of conflicting accesses (at least one write)\n"
                 "  to the same location from different threads, ensure HB via:\n"
                 "    · mutex / shared_mutex\n"
                 "    · atomic with sufficient memory_order\n"
                 "    · thread join / barrier / latch / etc.\n"
                 "  Otherwise: data race = UB.\n";

    std::cout << "[happens_before_intro] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/happens_before_intro", run>;

}  // namespace
