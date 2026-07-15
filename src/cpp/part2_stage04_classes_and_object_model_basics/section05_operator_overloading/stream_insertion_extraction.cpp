// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : stream_insertion_extraction
// Topic id : part2/stage04/section05/stream_insertion_extraction
//
// Covers: non-member << / >>, friend access, chaining

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ios>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace {

class Money {
    long cents_ = 0;

public:
    Money() = default;
    explicit Money(long cents) : cents_(cents) {}
    long cents() const { return cents_; }

    friend std::ostream& operator<<(std::ostream& os, const Money& m);
    friend std::istream& operator>>(std::istream& is, Money& m);
};

std::ostream& operator<<(std::ostream& os, const Money& m) {
    long dollars = m.cents_ / 100;
    long rem = m.cents_ % 100;
    if (rem < 0) {
        rem = -rem;
    }
    os << '$' << dollars << '.';
    if (rem < 10) {
        os << '0';
    }
    return os << rem;
}

std::istream& operator>>(std::istream& is, Money& m) {
    char dollar = 0;
    long dollars = 0;
    char dot = 0;
    long rem = 0;
    if (is >> dollar >> dollars >> dot >> rem && dollar == '$' && dot == '.') {
        m.cents_ = dollars * 100 + rem;
    } else {
        is.setstate(std::ios::failbit);
    }
    return is;
}

void demo_basics() {
    Money m{125};
    std::ostringstream oss;
    oss << m;
    assert(oss.str() == "$1.25");
}

void demo_intermediate() {
    std::istringstream iss("$2.05");
    Money m;
    iss >> m;
    assert(m.cents() == 205);
}

void demo_expert() {
    Money a{100};
    Money b{5};
    std::ostringstream oss;
    oss << a << ' ' << b;
    assert(oss.str() == "$1.00 $0.05");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/stream_insertion_extraction", run>;

}  // namespace
