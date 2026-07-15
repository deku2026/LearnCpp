// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section01_features_by_standard_cpp11
// Item     : cpp11_library_features_overview
// Topic id : part2/stage14/section01/cpp11_library_features_overview
//
// Covers: C++11 library feature sampler

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <chrono>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

void demo_basics() {
    std::unique_ptr<int> p(new int(7));
    assert(p && *p == 7);
    std::shared_ptr<int> s = std::make_shared<int>(3);
    assert(s.use_count() == 1);
}

void demo_intermediate() {
    auto t = std::make_tuple(1, 2.0, 'c');
    assert(std::get<0>(t) == 1);
    std::array<int, 3> a{{1, 2, 3}};
    assert(a.size() == 3);
    static_assert(std::is_integral_v<int> || std::is_integral<int>::value);
}

void demo_expert() {
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();
    auto t1 = clock::now();
    assert(t1 >= t0);
    std::unique_ptr<int[]> arr(new int[2]{1, 2});
    assert(arr[0] == 1 && arr[1] == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section01/cpp11_library_features_overview", run>;

}  // namespace
