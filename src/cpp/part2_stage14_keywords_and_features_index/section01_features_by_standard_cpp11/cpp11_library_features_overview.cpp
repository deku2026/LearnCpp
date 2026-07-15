// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section01_features_by_standard_cpp11
// Item     : cpp11_library_features_overview
// Topic id : part2/stage14/section01/cpp11_library_features_overview
//
// C++11 standard library micro-matrix (signature pieces that reshaped everyday C++).
// Related macros (after <version>): __cpp_lib_shared_ptr_arrays (later), but C++11 core
// is always available under -std=c++11+.

#include "learn/topic_registry.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <regex>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++11 library features micro-matrix ===\n";

    // 1) unique_ptr / shared_ptr / weak_ptr / make_shared
    auto up = std::unique_ptr<int>(new int{42});  // make_unique is C++14
    auto sp = std::make_shared<std::string>("shared");
    std::weak_ptr<std::string> wp = sp;
    assert(up && *up == 42);
    assert(!wp.expired());
    std::cout << "[smart pointers] *up=" << *up << " *sp=" << *sp << " weak_lock=" << *wp.lock() << '\n';

    // 2) move / forward / exchange-like pattern via swap+move
    std::string s1 = "hello";
    std::string s2 = std::move(s1);
    auto identity = [](int& x) -> int& { return x; };
    int n = 3;
    int& nr = identity(n);
    nr = 4;
    // perfect forward sample
    auto fwd = [](int&& v) { return std::forward<int>(v); };
    const int moved_val = fwd(7);
    std::cout << "[move/forward] s2=" << s2 << " n=" << n << " fwd=" << moved_val << '\n';

    // 3) tuple / tie / array
    auto t = std::make_tuple(1, std::string{"x"}, 2.5);
    int a = 0;
    std::string b;
    std::tie(a, b, std::ignore) = t;
    std::array<int, 3> arr{{10, 20, 30}};
    std::cout << "[tuple/array] a=" << a << " b=" << b << " arr[1]=" << arr[1] << '\n';
    assert(a == 1 && b == "x");

    // 4) unordered_map + begin/end free functions
    std::unordered_map<std::string, int> um{{"a", 1}, {"b", 2}};
    int um_sum = 0;
    for (auto it = std::begin(um); it != std::end(um); ++it) {
        um_sum += it->second;
    }
    std::cout << "[unordered_map/begin-end] sum=" << um_sum << " size=" << um.size() << '\n';
    assert(um_sum == 3);

    // 5) type_traits
    static_assert(std::is_integral<int>::value, "is_integral");
    static_assert(std::is_same<typename std::remove_reference<int&>::type, int>::value, "remove_ref");
    std::cout << "[type_traits] is_pod-style checks via is_trivial(int)=" << std::is_trivial<int>::value << '\n';

    // 6) function / bind / ref
    std::function<int(int)> twice = [](int x) { return x * 2; };
    auto plus = std::bind(std::plus<int>{}, std::placeholders::_1, 10);
    int base = 5;
    auto add_ref = std::bind(std::plus<int>{}, std::ref(base), 1);
    std::cout << "[function/bind/ref] twice(21)=" << twice(21) << " plus(7)=" << plus(7) << " add_ref=" << add_ref()
              << '\n';
    assert(twice(21) == 42);

    // 7) chrono
    using clock = std::chrono::steady_clock;
    const auto t0 = clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - t0);
    std::cout << "[chrono] elapsed_ms>=" << ms.count() << " (non-negative)\n";
    assert(ms.count() >= 0);

    // 8) thread / mutex / condition_variable / atomic
    std::atomic<int> counter{0};
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    std::thread worker([&] {
        {
            std::lock_guard<std::mutex> lk(m);
            ready = true;
        }
        cv.notify_one();
        counter.fetch_add(1, std::memory_order_relaxed);
    });
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return ready; });
    }
    worker.join();
    std::cout << "[thread/mutex/cv/atomic] counter=" << counter.load() << '\n';
    assert(counter == 1);

    // 9) async / future / promise
    std::promise<int> prom;
    std::future<int> fut_from_prom = prom.get_future();
    std::thread pth([&] { prom.set_value(99); });
    auto fut = std::async(std::launch::async, [] { return 41 + 1; });
    const int async_v = fut.get();
    const int prom_v = fut_from_prom.get();
    pth.join();
    std::cout << "[async/future/promise] async=" << async_v << " promise=" << prom_v << '\n';
    assert(async_v == 42 && prom_v == 99);

    // 10) random
    std::mt19937 eng{42};
    std::uniform_int_distribution<int> dist(1, 6);
    const int roll = dist(eng);
    std::cout << "[random] d6_roll=" << roll << '\n';
    assert(roll >= 1 && roll <= 6);

    // 11) regex + to_string
    const std::string text = "id=42";
    std::regex re(R"(id=(\d+))");
    std::smatch match;
    const bool ok = std::regex_search(text, match, re);
    const std::string as_str = std::to_string(42);
    std::cout << "[regex/to_string] ok=" << ok << " capt=" << (ok ? match[1].str() : "?") << " to_string=" << as_str
              << '\n';
    assert(ok && match[1] == "42" && as_str == "42");

    std::cout << "C++11 library matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section01/cpp11_library_features_overview", run>;

}  // namespace
