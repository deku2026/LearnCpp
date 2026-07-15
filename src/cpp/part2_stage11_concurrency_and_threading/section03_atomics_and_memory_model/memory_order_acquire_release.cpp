// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 8.3 / 8.5 验收点
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_acquire_release
// Topic id : part2/stage11/section03/memory_order_acquire_release
// Refs     : https://en.cppreference.com/w/cpp/atomic/memory_order
//            ISO [atomics.order] [intro.races]
//
// Core pattern: producer release-stores a flag; consumer acquire-loads it.
// If the consumer sees the producer's value, all writes before the release
// become visible after the acquire (synchronizes-with → happens-before).

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [acq/rel] publish non-atomic data via flag ===\n";
    {
        std::string data;
        std::atomic<bool> ready{false};

        std::jthread producer([&] {
            data = "Hello";  // ① non-atomic write
            // ② release: prior writes cannot reorder past this store;
            //    they become visible to an acquire that sees true.
            ready.store(true, std::memory_order_release);
        });

        std::jthread consumer([&] {
            while (!ready.load(std::memory_order_acquire)) {  // ③
                std::this_thread::yield();
            }
            // ④: ① happens-before ④
            assert(data == "Hello");
            std::cout << "  consumer saw data=\"" << data << "\"\n";
        });
    }

    std::cout << "=== why not seq_cst here? ===\n";
    std::cout << "  Only one flag pairs one producer with consumers.\n"
                 "  We need happens-before for the payload, not a global total\n"
                 "  order among many seq_cst atomics. acq/rel is exactly enough.\n";

    std::cout << "=== mutex is also release-acquire ===\n";
    std::cout << "  unlock ≈ release, lock ≈ acquire — same visibility story.\n";

    std::cout << "=== multi-payload publish ===\n";
    {
        int x = 0;
        int y = 0;
        std::atomic<bool> ready{false};

        std::jthread prod([&] {
            x = 11;
            y = 22;
            ready.store(true, std::memory_order_release);
        });
        std::jthread cons([&] {
            while (!ready.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            assert(x == 11 && y == 22);
        });
        std::cout << "  both x and y visible after acquire sees true\n";
    }

    std::cout << "[memory_order_acquire_release] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_acquire_release", run>;

}  // namespace
