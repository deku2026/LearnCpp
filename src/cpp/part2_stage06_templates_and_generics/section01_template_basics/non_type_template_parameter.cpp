// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : non_type_template_parameter
// Topic id : part2/stage06/section01/non_type_template_parameter
//
// Covers: NTTP (size, pointer-to-member style values), array-like fixed capacity

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>

namespace {

template <typename T, std::size_t N>
struct FixedBuffer {
    std::array<T, N> data{};
    std::size_t used = 0;

    void push(const T& v) {
        assert(used < N);
        data[used++] = v;
    }

    T& operator[](std::size_t i) {
        assert(i < used);
        return data[i];
    }

    const T& operator[](std::size_t i) const {
        assert(i < used);
        return data[i];
    }

    static constexpr std::size_t capacity() { return N; }
};

template <int N>
constexpr int factorial() {
    if constexpr (N <= 1) {
        return 1;
    } else {
        return N * factorial<N - 1>();
    }
}

template <auto V>
constexpr auto identity_value() {
    return V;
}

void demo_basics() {
    FixedBuffer<int, 3> buf;
    buf.push(10);
    buf.push(20);
    assert(buf.used == 2);
    assert(buf[0] == 10);
    assert((FixedBuffer<int, 3>::capacity() == 3));
}

void demo_intermediate() {
    static_assert(factorial<5>() == 120);
    assert(factorial<5>() == 120);

    static_assert(identity_value<42>() == 42);
    static_assert(identity_value<true>() == true);
}

void demo_expert() {
    // Different N => different types.
    static_assert(!std::is_same_v<FixedBuffer<int, 2>, FixedBuffer<int, 3>>);

    FixedBuffer<char, 4> letters;
    letters.push('a');
    letters.push('b');
    assert(letters[1] == 'b');

    constexpr auto n = identity_value<std::size_t{8}>();
    static_assert(n == 8);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/non_type_template_parameter", run>;

}  // namespace
