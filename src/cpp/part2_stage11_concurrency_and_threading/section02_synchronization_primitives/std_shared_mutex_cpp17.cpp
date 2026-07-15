// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_shared_mutex_cpp17
// Topic id : part2/stage11/section02/std_shared_mutex_cpp17
//
// Covers: std::shared_mutex readers-writer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
    std::shared_mutex sm;
    int data = 0;
    {
        std::unique_lock<std::shared_mutex> w(sm);
        data = 10;
    }
    {
        std::shared_lock<std::shared_mutex> r(sm);
        assert(data == 10);
    }
}

void demo_intermediate() {
    std::shared_mutex sm;
    int data = 1;
    std::vector<int> reads;
    std::mutex out_m;
    auto reader = [&] {
        std::shared_lock<std::shared_mutex> r(sm);
        int v = data;
        std::lock_guard<std::mutex> g(out_m);
        reads.push_back(v);
    };
    std::thread t1(reader);
    std::thread t2(reader);
    t1.join();
    t2.join();
    assert(reads.size() == 2);
    assert(reads[0] == 1 && reads[1] == 1);
}

void demo_expert() {
    std::shared_mutex sm;
    int data = 0;
    std::thread writer([&] {
        std::unique_lock<std::shared_mutex> w(sm);
        data = 42;
    });
    writer.join();
    int seen = 0;
    std::thread reader([&] {
        std::shared_lock<std::shared_mutex> r(sm);
        seen = data;
    });
    reader.join();
    assert(seen == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_shared_mutex_cpp17", run>;

}  // namespace
