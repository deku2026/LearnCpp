// Topic    : headers map · 并发 concurrency
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : concurrency_headers
// Topic id : part3/section08/concurrency_headers
// Headers  : thread atomic mutex shared_mutex condition_variable
//            future semaphore latch barrier stop_token

#include "learn/topic_registry.hpp"

#include <atomic>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

#if defined(__has_include)
#if __has_include(<semaphore>)
#include <semaphore>
#endif
#if __has_include(<latch>)
#include <latch>
#endif
#if __has_include(<barrier>)
#include <barrier>
#endif
#if __has_include(<stop_token>)
#include <stop_token>
#endif
#if __has_include(<shared_mutex>)
#include <shared_mutex>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [concurrency_headers] map probe ===\n";
    std::cout << "<thread> <atomic> <mutex> <shared_mutex>\n";
    std::cout << "<condition_variable> <future>\n";
    std::cout << "<semaphore> <latch> <barrier> <stop_token> (C++20)\n";

    std::atomic<int> counter{0};
    std::mutex m;
    int guarded = 0;

    std::thread t([&] {
        counter.fetch_add(1, std::memory_order_relaxed);
        std::lock_guard lock(m);
        ++guarded;
    });
    t.join();
    assert(counter.load() == 1);
    assert(guarded == 1);
    std::cout << "thread+atomic+mutex ok\n";

    auto fut = std::async(std::launch::async, [] { return 42; });
    assert(fut.get() == 42);
    std::cout << "future/async ok\n";

#if defined(__cpp_lib_semaphore)
    std::counting_semaphore<1> sem{1};
    sem.acquire();
    sem.release();
    std::cout << "semaphore ok\n";
#endif

#if defined(__cpp_lib_latch)
    std::latch done{1};
    done.count_down();
    done.wait();
    std::cout << "latch ok\n";
#endif

#if defined(__cpp_lib_jthread)
    {
        std::jthread jt([](std::stop_token) {
            // cooperative stop available via token
        });
    }
    std::cout << "jthread/stop_token ok\n";
#endif

    std::cout << "[concurrency_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/concurrency_headers", run>;

}  // namespace
