// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 4.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_shared_mutex_cpp17
// Topic id : part2/stage11/section02/std_shared_mutex_cpp17
// Refs     : https://en.cppreference.com/w/cpp/thread/shared_mutex
//            https://en.cppreference.com/w/cpp/thread/shared_lock
//            ISO [thread.sharedmutex.class]
//
// shared_mutex (C++17): many concurrent readers OR one exclusive writer.
// Pays off when reads dominate; otherwise plain mutex may be simpler/faster.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [shared_mutex] multi-reader / single-writer ===\n";
    {
        std::shared_mutex rw;
        int data = 0;

        auto reader = [&](int /*id*/) {
            for (int i = 0; i < 50; ++i) {
                std::shared_lock lock(rw);  // shared ownership
                (void)data;                 // observe under shared lock
            }
        };

        auto writer = [&] {
            for (int i = 0; i < 20; ++i) {
                std::unique_lock lock(rw);  // exclusive
                ++data;
            }
        };

        {
            std::vector<std::jthread> pool;
            pool.emplace_back(writer);
            for (int i = 0; i < 4; ++i) {
                pool.emplace_back(reader, i);
            }
        }
        assert(data == 20);
        std::cout << "  final data=" << data << '\n';
    }

    std::cout << "=== write exclusivity: readers wait for writer ===\n";
    {
        std::shared_mutex rw;
        int phase = 0;

        std::jthread writer([&] {
            std::unique_lock lock(rw);
            phase = 1;
            std::this_thread::sleep_for(30ms);
            phase = 2;
        });

        std::this_thread::sleep_for(5ms);  // let writer grab exclusive
        {
            std::shared_lock lock(rw);
            // If we got the shared lock, writer finished exclusive section.
            assert(phase == 2);
            std::cout << "  reader saw phase=2 after writer released\n";
        }
    }

    std::cout << "rule of thumb: use shared_mutex only when read >> write\n";
    std::cout << "[std_shared_mutex_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_shared_mutex_cpp17", run>;

}  // namespace
