// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : template_template_parameter
// Topic id : part2/stage06/section01/template_template_parameter
//
// Covers: template template parameters for container policy injection

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>
#include <list>
#include <vector>

namespace {

template <typename T, template <typename...> class Container>
class StackAdapter {
    Container<T> data_;

public:
    void push(const T& v) { data_.push_back(v); }
    void pop() {
        assert(!data_.empty());
        data_.pop_back();
    }
    T& top() {
        assert(!data_.empty());
        return data_.back();
    }
    std::size_t size() const { return data_.size(); }
};

template <template <typename...> class Seq>
int sum_first_three() {
    Seq<int> s{1, 2, 3, 4};
    int sum = 0;
    int n = 0;
    for (int v : s) {
        sum += v;
        if (++n == 3) {
            break;
        }
    }
    return sum;
}

void demo_basics() {
    StackAdapter<int, std::vector> sv;
    sv.push(10);
    sv.push(20);
    assert(sv.top() == 20);
    sv.pop();
    assert(sv.top() == 10);
}

void demo_intermediate() {
    StackAdapter<int, std::deque> sd;
    sd.push(1);
    sd.push(2);
    assert(sd.size() == 2);

    assert(sum_first_three<std::vector>() == 6);
    assert(sum_first_three<std::list>() == 6);
}

void demo_expert() {
    // Policy choice: vector vs deque changes storage, same adapter API.
    StackAdapter<char, std::vector> sc;
    sc.push('a');
    sc.push('b');
    assert(sc.top() == 'b');
    assert(sc.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/template_template_parameter", run>;

}  // namespace
