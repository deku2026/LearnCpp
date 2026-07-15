// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 4.2 / 4.4
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : scoped_lock_cpp17
// Topic id : part2/stage11/section02/scoped_lock_cpp17
// Refs     : https://en.cppreference.com/w/cpp/thread/scoped_lock
//            ISO [thread.lock.scoped]
//
// scoped_lock (C++17): lock zero or more mutexes with deadlock avoidance
// (same algorithm family as std::lock). Prefer over hand-ordered multi-lock.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [scoped_lock] single mutex (like lock_guard) ===\n";
    {
        std::mutex m;
        int v = 0;
        {
            std::scoped_lock lock(m);
            ++v;
        }
        assert(v == 1);
    }

    std::cout << "=== multi-mutex without deadlock (different orders) ===\n";
    {
        std::mutex m1;
        std::mutex m2;
        int a = 0;
        int b = 0;

        // Without scoped_lock, t1 locks m1 then m2 while t2 locks m2 then m1
        // can deadlock. scoped_lock acquires both safely either way.
        auto left_to_right = [&] {
            for (int i = 0; i < 200; ++i) {
                std::scoped_lock lock(m1, m2);
                ++a;
                ++b;
            }
        };
        auto right_to_left = [&] {
            for (int i = 0; i < 200; ++i) {
                std::scoped_lock lock(m2, m1);  // opposite argument order
                ++a;
                ++b;
            }
        };

        {
            std::jthread t1(left_to_right);
            std::jthread t2(right_to_left);
        }
        assert(a == 400);
        assert(b == 400);
        std::cout << "  a=b=" << a << " (no deadlock under crossed order)\n";
    }

    std::cout << "=== transfer between two accounts ===\n";
    {
        std::mutex ma;
        std::mutex mb;
        int account_a = 100;
        int account_b = 50;

        auto transfer = [&](int amount) {
            std::scoped_lock lock(ma, mb);
            account_a -= amount;
            account_b += amount;
        };

        {
            std::jthread t1([&] { transfer(10); });
            std::jthread t2([&] { transfer(5); });
        }
        assert(account_a == 85);
        assert(account_b == 65);
        std::cout << "  A=" << account_a << " B=" << account_b << '\n';
    }

    std::cout << "[scoped_lock_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/scoped_lock_cpp17", run>;

}  // namespace
