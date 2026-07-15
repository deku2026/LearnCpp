// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 2 / 5.3 优雅关闭
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : graceful_shutdown
// Topic id : part2/stage11/section04/graceful_shutdown
// Refs     : https://en.cppreference.com/w/cpp/thread/jthread
//            https://en.cppreference.com/w/cpp/thread/condition_variable_any
//
// Graceful shutdown: request stop → workers leave at safe points → join.
// Combine stop_token with condition_variable_any for interruptible waits.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [graceful_shutdown] jthread workers + stop_token ===\n";
    {
        std::atomic<int> units{0};
        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < 3; ++i) {
                pool.emplace_back([&units](std::stop_token st) {
                    while (!st.stop_requested()) {
                        units.fetch_add(1, std::memory_order_relaxed);
                        std::this_thread::sleep_for(8ms);
                    }
                });
            }
            std::this_thread::sleep_for(40ms);
            // pool dtor: each jthread request_stop + join
        }
        assert(units.load() >= 3);
        std::cout << "  work units before stop=" << units.load() << '\n';
    }

    std::cout << "=== interruptible queue drain with condition_variable_any ===\n";
    {
        std::mutex mtx;
        std::condition_variable_any cv;
        std::queue<int> q;
        std::atomic<int> processed{0};

        std::jthread consumer([&](std::stop_token st) {
            while (true) {
                std::unique_lock lock(mtx);
                // Returns false if stop requested while pred is still false.
                const bool has_work = cv.wait(lock, st, [&] { return !q.empty(); });
                if (!has_work) {
                    break;  // cancelled
                }
                const int item = q.front();
                q.pop();
                lock.unlock();
                (void)item;
                processed.fetch_add(1, std::memory_order_relaxed);
            }
        });

        {
            std::lock_guard lock(mtx);
            for (int i = 0; i < 5; ++i) {
                q.push(i);
            }
        }
        cv.notify_all();
        std::this_thread::sleep_for(20ms);
        consumer.request_stop();
        cv.notify_all();  // belt-and-suspenders; wait(st) also wakes on stop
        consumer.join();

        assert(processed.load() == 5);
        std::cout << "  processed=" << processed.load() << " then stop interrupted empty wait\n";
    }

    std::cout << "=== pipeline: producers stop, consumers drain, then exit ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<int> buffer;
        bool producers_done = false;
        std::atomic<int> got{0};

        auto producer = [&](std::stop_token st, int base) {
            for (int i = 0; i < 10 && !st.stop_requested(); ++i) {
                {
                    std::lock_guard lock(mtx);
                    buffer.push(base + i);
                }
                cv.notify_one();
                std::this_thread::sleep_for(2ms);
            }
        };

        auto consumer = [&] {
            while (true) {
                std::unique_lock lock(mtx);
                cv.wait(lock, [&] { return !buffer.empty() || producers_done; });
                if (buffer.empty() && producers_done) {
                    break;
                }
                buffer.pop();
                lock.unlock();
                got.fetch_add(1, std::memory_order_relaxed);
            }
        };

        {
            std::jthread p1(producer, 0);
            std::jthread p2(producer, 100);
            std::jthread c1(consumer);
            std::jthread c2(consumer);
            std::this_thread::sleep_for(30ms);
            p1.request_stop();
            p2.request_stop();
            p1.join();
            p2.join();
            {
                std::lock_guard lock(mtx);
                producers_done = true;
            }
            cv.notify_all();
            // consumers join on scope exit
        }
        assert(got.load() > 0);
        std::cout << "  drained " << got.load() << " items after producer stop\n";
    }

    std::cout << "[graceful_shutdown] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section04/graceful_shutdown", run>;

}  // namespace
