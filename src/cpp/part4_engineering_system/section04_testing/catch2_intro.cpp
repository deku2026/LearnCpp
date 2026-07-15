// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.1 Catch2
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : catch2_intro
// Topic id : part4/section04/catch2_intro
//
// 要点: header-only、REQUIRE/CHECK、BDD 风格 SCENARIO/GIVEN/WHEN/THEN。
//       迷你实现语义, 不依赖 Catch2 头。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct CatchStats {
    int assertions = 0;
    int failures = 0;
};

CatchStats g{};

#define CHECK(expr)                                        \
    do {                                                   \
        ++g.assertions;                                    \
        if (!(expr)) {                                     \
            ++g.failures;                                  \
            std::cout << "  CHECK failed: " #expr << '\n'; \
        }                                                  \
    } while (0)

#define REQUIRE(expr)                                        \
    do {                                                     \
        ++g.assertions;                                      \
        if (!(expr)) {                                       \
            ++g.failures;                                    \
            std::cout << "  REQUIRE failed: " #expr << '\n'; \
            throw std::runtime_error("REQUIRE failed");      \
        }                                                    \
    } while (0)

// 被测: 栈
class IntStack {
public:
    void push(int x) { data_.push_back(x); }
    int pop() {
        int v = data_.back();
        data_.pop_back();
        return v;
    }
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

void scenario_stack_lifo() {
    // SCENARIO: stack is LIFO
    IntStack s;  // GIVEN empty stack
    CHECK(s.empty());

    s.push(1);  // WHEN push 1 then 2
    s.push(2);
    CHECK(s.size() == 2);

    // THEN pop yields 2 then 1
    CHECK(s.pop() == 2);
    CHECK(s.pop() == 1);
    CHECK(s.empty());
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== catch2_intro (CHECK/REQUIRE + BDD sketch) ===\n";
    g = {};
    scenario_stack_lifo();
    assert(g.failures == 0);
    assert(g.assertions >= 5);
    std::cout << "  assertions=" << g.assertions << '\n';

    // REQUIRE 抛出中止 section; CHECK 继续
    g = {};
    try {
        REQUIRE(1 == 2);
        CHECK(true);
    } catch (const std::runtime_error&) {
        // expected
    }
    assert(g.failures == 1);
    std::cout << "  REQUIRE aborts section via exception style\n";

    // SECTION 思想: 同一 TEST_CASE 多个分支从共同 setup 分叉
    // Catch2: TEST_CASE("x") { setup; SECTION("a"){...} SECTION("b"){...} }
    int setup_count = 0;
    auto section = [&](auto fn) {
        ++setup_count;  // 每 section 重新 setup
        IntStack s;
        s.push(10);
        fn(s);
    };
    section([](IntStack& s) { CHECK(s.pop() == 10); });
    section([](IntStack& s) {
        s.push(20);
        CHECK(s.size() == 2);
    });
    assert(setup_count == 2);
    std::cout << "  SECTION replays setup per branch\n";

    std::cout << "catch2_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/catch2_intro", run>;

}  // namespace
