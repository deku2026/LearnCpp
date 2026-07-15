// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 5 / 验收点
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_condition_variable
// Topic id : part2/stage11/section02/std_condition_variable
// Refs     : https://en.cppreference.com/w/cpp/thread/condition_variable
//            ISO [thread.condition.condvar]
//
// condition_variable: sleep until a predicate becomes true (no busy spin).
// Iron rule: ALWAYS wait with a predicate (spurious wakeups are real).
// Acceptance: bounded producer-consumer with graceful shutdown.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
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

    std::cout << "=== [condition_variable] wait with predicate ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;
        int payload = 0;

        std::jthread producer([&] {
            std::this_thread::sleep_for(15ms);
            {
                std::lock_guard lock(mtx);
                payload = 42;
                ready = true;
            }
            cv.notify_one();
        });

        std::unique_lock lock(mtx);
        // Equivalent to: while (!ready) cv.wait(lock);
        cv.wait(lock, [&] { return ready; });
        assert(payload == 42);
        std::cout << "  payload=" << payload << '\n';
    }

    std::cout << "=== bounded producer-consumer + graceful done ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<int> buffer;
        bool done = false;
        constexpr std::size_t kMax = 8;
        constexpr int kItems = 30;

        std::atomic<int> consumed{0};  // atomic only for end check across threads
        // consumed is updated under mutex below; atomic is overkill but harmless.

        auto producer = [&] {
            for (int i = 0; i < kItems; ++i) {
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [&] { return buffer.size() < kMax; });
                    buffer.push(i);
                }
                cv.notify_all();
            }
            {
                std::lock_guard lock(mtx);
                done = true;
            }
            cv.notify_all();
        };

        auto consumer = [&](int /*id*/) {
            while (true) {
                int item = 0;
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [&] { return !buffer.empty() || done; });
                    if (buffer.empty() && done) {
                        break;
                    }
                    item = buffer.front();
                    buffer.pop();
                }
                cv.notify_all();  // free a slot for producers
                (void)item;
                consumed.fetch_add(1, std::memory_order_relaxed);
            }
        };

        {
            std::jthread p(producer);
            std::jthread c1(consumer, 1);
            std::jthread c2(consumer, 2);
        }

        assert(consumed.load() == kItems);
        {
            std::lock_guard lock(mtx);
            assert(buffer.empty());
            assert(done);
        }
        std::cout << "  consumed=" << consumed.load() << " buffer empty, done\n";
    }

    std::cout << "never: bare cv.wait(lock) without predicate\n";
    std::cout << "[std_condition_variable] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_condition_variable", run>;

}  // namespace
