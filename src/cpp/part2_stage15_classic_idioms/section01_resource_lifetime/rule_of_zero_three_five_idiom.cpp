// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : rule_of_zero_three_five_idiom
// Topic id : part2/stage15/section01/rule_of_zero_three_five_idiom
//
// Covers: Rule of 0/3/5

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>

namespace {

struct Rule0 {
    std::string name;
    std::unique_ptr<int> data;
};

struct Rule5 {
    int* p;
    explicit Rule5(int v) : p(new int(v)) {}
    ~Rule5() { delete p; }
    Rule5(const Rule5& o) : p(new int(*o.p)) {}
    Rule5& operator=(const Rule5& o) {
        if (this != &o) {
            auto* n = new int(*o.p);
            delete p;
            p = n;
        }
        return *this;
    }
    Rule5(Rule5&& o) noexcept : p(o.p) { o.p = nullptr; }
    Rule5& operator=(Rule5&& o) noexcept {
        if (this != &o) {
            delete p;
            p = o.p;
            o.p = nullptr;
        }
        return *this;
    }
};

void demo_basics() {
    Rule0 a{"a", std::make_unique<int>(1)};
    Rule0 b = std::move(a);
    assert(b.name == "a");
    assert(b.data && *b.data == 1);
}

void demo_intermediate() {
    Rule5 x(3);
    Rule5 y = x;
    assert(*y.p == 3);
    assert(*x.p == 3);
}

void demo_expert() {
    Rule5 x(5);
    Rule5 y(0);
    y = std::move(x);
    assert(y.p && *y.p == 5);
    assert(x.p == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/rule_of_zero_three_five_idiom", run>;

}  // namespace
