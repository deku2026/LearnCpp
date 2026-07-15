// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : deduction_guides
// Topic id : part2/stage06/section03/deduction_guides
//
// Covers: user-defined deduction guides for CTAD

#include "learn/topic_registry.hpp"

#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <typename T>
struct Wrapper {
    T value;
    explicit Wrapper(T v) : value(std::move(v)) {}
};

// Guide: deduce T from constructor argument as decayed type.
Wrapper(const char*) -> Wrapper<std::string>;

template <typename T>
struct SpanLike {
    T* data;
    std::size_t n;
    SpanLike(T* p, std::size_t len) : data(p), n(len) {}

    template <std::size_t N>
    explicit SpanLike(T (&arr)[N]) : data(arr), n(N) {}
};

template <typename T, std::size_t N>
SpanLike(T (&)[N]) -> SpanLike<T>;

template <typename It>
struct Range {
    It begin_;
    It end_;
    Range(It b, It e) : begin_(b), end_(e) {}
    auto size() const { return static_cast<std::size_t>(std::distance(begin_, end_)); }
};

template <typename It>
Range(It, It) -> Range<It>;

void demo_basics() {
    Wrapper w("hello");  // uses guide -> Wrapper<string>
    LEARN_CHECK(w.value == "hello");
    static_assert(std::is_same_v<decltype(w), Wrapper<std::string>>);
}

void demo_intermediate() {
    int arr[3] = {1, 2, 3};
    SpanLike s(arr);
    LEARN_CHECK(s.n == 3);
    LEARN_CHECK(s.data[0] == 1);
    static_assert(std::is_same_v<decltype(s), SpanLike<int>>);
}

void demo_expert() {
    std::vector<int> v{10, 20, 30};
    Range r(v.begin(), v.end());
    LEARN_CHECK(r.size() == 3);
    static_assert(std::is_same_v<decltype(r), Range<std::vector<int>::iterator>>);

    Wrapper w2(42);
    LEARN_CHECK(w2.value == 42);
    static_assert(std::is_same_v<decltype(w2), Wrapper<int>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/deduction_guides", run>;

}  // namespace
