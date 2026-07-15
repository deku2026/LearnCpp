// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : default_template_arguments
// Topic id : part2/stage06/section01/default_template_arguments
//
// Covers: default type/NTTP template arguments, allocator-style defaults

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <typename T, typename Allocator = std::allocator<T>>
struct SimpleVec {
    using allocator_type = Allocator;
    std::vector<T, Allocator> data;

    void push(const T& v) { data.push_back(v); }
    std::size_t size() const { return data.size(); }
};

template <typename T = int, int N = 4>
struct ArrayLike {
    T values[N]{};
    static constexpr int size() { return N; }
};

template <typename T, typename U = T>
U convert_or_same(T v) {
    return static_cast<U>(v);
}

void demo_basics() {
    SimpleVec<int> v;  // Allocator defaults to std::allocator<int>
    v.push(1);
    v.push(2);
    assert(v.size() == 2);
    static_assert(std::is_same_v<SimpleVec<int>::allocator_type, std::allocator<int>>);
}

void demo_intermediate() {
    ArrayLike<> a;  // T=int, N=4
    a.values[0] = 7;
    assert(a.values[0] == 7);
    static_assert(ArrayLike<>::size() == 4);

    ArrayLike<double, 2> d;
    d.values[1] = 1.5;
    assert(d.values[1] == 1.5);
    static_assert(ArrayLike<double, 2>::size() == 2);
}

void demo_expert() {
    assert(convert_or_same(3) == 3);
    assert((convert_or_same<int, double>(3) == 3.0));

    SimpleVec<std::string> ss;
    ss.push("x");
    assert(ss.data[0] == "x");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/default_template_arguments", run>;

}  // namespace
