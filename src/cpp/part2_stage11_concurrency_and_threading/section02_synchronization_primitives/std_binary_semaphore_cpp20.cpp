// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 6.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_binary_semaphore_cpp20
// Topic id : part2/stage11/section02/std_binary_semaphore_cpp20
// Refs     : https://en.cppreference.com/w/cpp/thread/counting_semaphore
//            binary_semaphore = counting_semaphore<1>
//
// binary_semaphore: 0/1 signalling between threads (ping-pong / event).

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [binary_semaphore] event signal ===\n";
    {
        std::binary_semaphore go{0};  // starts closed
        std::atomic<int> order{0};

        std::jthread worker([&] {
            go.acquire();  // wait for signal
            order.store(2, std::memory_order_relaxed);
        });

        order.store(1, std::memory_order_relaxed);
        go.release();  // open the gate once
        worker.join();
        assert(order.load() == 2);
        std::cout << "  signalled worker proceeded\n";
    }

    std::cout << "=== ping-pong rendezvous ===\n";
    {
        std::binary_semaphore ping{1};
        std::binary_semaphore pong{0};
        std::vector<char> trace;
        std::mutex trace_mtx;

        auto record = [&](char c) {
            std::lock_guard lock(trace_mtx);
            trace.push_back(c);
        };

        {
            std::jthread a([&] {
                for (int i = 0; i < 4; ++i) {
                    ping.acquire();
                    record('A');
                    pong.release();
                }
            });
            std::jthread b([&] {
                for (int i = 0; i < 4; ++i) {
                    pong.acquire();
                    record('B');
                    ping.release();
                }
            });
        }

        assert(trace.size() == 8);
        for (std::size_t i = 0; i < trace.size(); i += 2) {
            assert(trace[i] == 'A');
            assert(trace[i + 1] == 'B');
        }
        std::cout << "  ABABABAB alternating handshake\n";
    }

    static_assert(std::binary_semaphore::max() >= 1);
    std::cout << "[std_binary_semaphore_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_binary_semaphore_cpp20", run>;

}  // namespace
