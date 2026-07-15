// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.5 fuzztest (Google)
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : fuzztest_intro
// Topic id : part4/section04/fuzztest_intro
//
// 要点: 属性测试 + 领域生成器(字符串/整数/容器); 与 gtest 风格接近。
//       此处手写 domain 生成 + 属性断言, 不链 fuzztest。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

// 被测属性: reverse 两次还原; sort 后有序
[[maybe_unused]] std::string rev(std::string s) {
    std::reverse(s.begin(), s.end());
    return s;
}

// 简易 PRNG
struct Rng {
    std::uint32_t s;
    std::uint32_t next() {
        s = s * 1664525u + 1013904223u;
        return s;
    }
    int uniform(int lo, int hi) { return lo + static_cast<int>(next() % static_cast<std::uint32_t>(hi - lo + 1)); }
};

std::string gen_string(Rng& r, int max_len) {
    const int n = r.uniform(0, max_len);
    std::string s;
    s.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        s.push_back(static_cast<char>(r.uniform(32, 126)));
    }
    return s;
}

std::vector<int> gen_vec(Rng& r, int max_n) {
    const int n = r.uniform(0, max_n);
    std::vector<int> v;
    v.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        v.push_back(r.uniform(-50, 50));
    }
    return v;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== fuzztest_intro (property-based sketch) ===\n";

    Rng rng{0xC0FFEEu};
    int props = 0;

    // Property: reverse is involution
    for (int i = 0; i < 100; ++i) {
        auto s = gen_string(rng, 32);
        assert(rev(rev(s)) == s);
        ++props;
    }

    // Property: sort 后单调 + 元素多重集不变
    for (int i = 0; i < 100; ++i) {
        auto v = gen_vec(rng, 20);
        auto sorted = v;
        std::sort(sorted.begin(), sorted.end());
        assert(std::is_sorted(sorted.begin(), sorted.end()));
        auto a = v;
        auto b = sorted;
        std::sort(a.begin(), a.end());
        assert(a == b);
        ++props;
    }

    // Property: sum 与 empty
    {
        std::vector<int> empty;
        assert(std::accumulate(empty.begin(), empty.end(), 0) == 0);
        ++props;
    }

    assert(props == 201);
    std::cout << "  properties checked on random domains, count=" << props << '\n';

    // fuzztest 风格: FUZZ_TEST(Suite, Name).WithDomains(...)
    std::cout << "  real fuzztest: FUZZ_TEST + WithDomains(Arbitrary<T>())\n";
    std::cout << "fuzztest_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/fuzztest_intro", run>;

}  // namespace
