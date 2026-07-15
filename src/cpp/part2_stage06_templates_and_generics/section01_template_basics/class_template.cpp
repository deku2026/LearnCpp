// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : class_template
// Topic id : part2/stage06/section01/class_template
//
// Covers: class templates, per-T monomorphization, on-demand member instantiation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace {

template <typename T>
class Stack {
    std::vector<T> data_;

public:
    void push(const T& value) { data_.push_back(value); }
    void push(T&& value) { data_.push_back(std::move(value)); }

    T pop() {
        assert(!data_.empty());
        T v = std::move(data_.back());
        data_.pop_back();
        return v;
    }

    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
    const T& top() const {
        assert(!data_.empty());
        return data_.back();
    }
};

void demo_basics() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    assert(s.size() == 2);
    assert(s.top() == 2);
    assert(s.pop() == 2);
    assert(s.pop() == 1);
    assert(s.empty());
}

void demo_intermediate() {
    Stack<std::string> ss;
    ss.push("alpha");
    ss.push(std::string{"beta"});
    assert(ss.top() == "beta");
    assert(ss.pop() == "beta");
    assert(ss.pop() == "alpha");
}

void demo_expert() {
    // Stack<int> and Stack<double> are distinct types.
    Stack<int> si;
    Stack<double> sd;
    si.push(1);
    sd.push(1.5);
    assert(si.size() == 1);
    assert(sd.top() == 1.5);

    // Distinct monomorphs; member functions instantiate on use only.
    Stack<long> sl;
    sl.push(100L);
    assert(sl.top() == 100L);
    assert(sl.size() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/class_template", run>;

}  // namespace
