// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section04_features_by_standard_cpp20
// Item     : cpp20_library_features_overview
// Topic id : part2/stage14/section04/cpp20_library_features_overview
//
// C++20 library micro-matrix: ranges/views, format, span, jthread/stop_token,
// <bit>/bit_cast, numbers, is_constant_evaluated, midpoint/lerp, to_array,
// bind_front, erase/erase_if, latch, source_location, string starts_with, ssize.

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <format>
#include <functional>
#include <iostream>
#include <latch>
#include <numbers>
#include <numeric>
#include <ranges>
#include <source_location>
#include <span>
#include <stop_token>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>
#include <version>

namespace {

constexpr int ice_demo(int x) {
    if (std::is_constant_evaluated()) {
        return x + 100;
    }
    return x + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++20 library features micro-matrix ===\n";

    // 1) ranges / views
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto even_sq =
        v | std::views::filter([](int x) { return x % 2 == 0; }) | std::views::transform([](int x) { return x * x; });
    std::vector<int> material;
    for (int x : even_sq) {
        material.push_back(x);
    }
    std::cout << "[ranges] even squares:";
    for (int x : material) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
    assert(material.size() == 3 && material[0] == 4);

    // 2) format
    const std::string msg = std::format("Hello, {}! answer={}", "C++20", 42);
    std::cout << "[format] " << msg << '\n';
    assert(msg.find("42") != std::string::npos);

    // 3) span
    std::array<int, 4> arr{1, 2, 3, 4};
    std::span<int> sp(arr);
    sp[1] = 20;
    std::cout << "[span] sp[1]=" << sp[1] << " size=" << sp.size() << '\n';
    assert(arr[1] == 20);

    // 4) jthread / stop_token
    std::atomic<int> ticks{0};
    {
        std::jthread jt([&](std::stop_token st) {
            while (!st.stop_requested()) {
                ticks.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::yield();
            }
        });
        // jthread destructor requests stop + joins
    }
    std::cout << "[jthread/stop_token] ticks=" << ticks.load() << '\n';

    // 5) bit / bit_cast / numbers
    const unsigned u = 0b1010u;
    std::cout << "[bit] popcount=" << std::popcount(u)
              << " endian_native_is_little=" << (std::endian::native == std::endian::little) << '\n';
    const float f = 1.0f;
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
    std::cout << "  bit_cast(1.0f)=" << bits << " pi≈" << std::numbers::pi << '\n';
    assert(std::popcount(u) == 2);

    // 6) is_constant_evaluated / midpoint / lerp / to_array
    constexpr int ice_c = ice_demo(1);
    volatile int ice_in = 1;  // force runtime evaluation of is_constant_evaluated path
    const int ice_r = ice_demo(ice_in);
    const int mid = std::midpoint(10, 20);
    const double ler = std::lerp(0.0, 10.0, 0.5);
    auto ta = std::to_array({1, 2, 3});
    std::cout << "[ice/mid/lerp/to_array] ice_c=" << ice_c << " ice_r=" << ice_r << " mid=" << mid << " lerp=" << ler
              << " ta[2]=" << ta[2] << '\n';
    assert(ice_c == 101 && ice_r == 2 && mid == 15);

    // 7) bind_front / erase / erase_if
    auto add = [](int a, int b) { return a + b; };
    auto add10 = std::bind_front(add, 10);
    std::vector<int> xs{1, 2, 3, 2, 4};
    std::erase(xs, 2);
    std::erase_if(xs, [](int n) { return n > 3; });
    std::cout << "[bind_front/erase] add10(5)=" << add10(5) << " xs.size=" << xs.size() << '\n';
    assert(add10(5) == 15 && xs.size() == 2);

    // 8) latch
    std::latch done{1};
    std::jthread waiter([&] { done.count_down(); });
    done.wait();
    std::cout << "[latch] opened\n";

    // 9) source_location / ssize / string starts_with/ends_with / contains-ish
    const auto loc = std::source_location::current();
    std::string_view name = "feature_test";
    std::cout << "[source_location] " << loc.file_name() << ':' << loc.line() << " func=" << loc.function_name()
              << '\n';
    std::cout << "  ssize(v)=" << std::ssize(v) << " starts=" << name.starts_with("feat")
              << " ends=" << name.ends_with("test") << '\n';
    assert(name.starts_with("feat") && name.ends_with("test"));

    // 10) concepts library (std::same_as etc.)
    static_assert(std::same_as<int, std::remove_cvref_t<const int&>>);
    std::cout << "[concepts lib] same_as check OK\n";

#if defined(__cpp_lib_syncbuf)
    std::cout << "[osyncstream] __cpp_lib_syncbuf=" << __cpp_lib_syncbuf << '\n';
#endif

    std::cout << "C++20 library matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section04/cpp20_library_features_overview", run>;

}  // namespace
