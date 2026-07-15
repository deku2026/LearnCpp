// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : copy_and_swap_strong_guarantee
// Topic id : part2/stage09/section03/copy_and_swap_strong_guarantee
//
// Covers: copy-and-swap assignment for strong exception safety

#include "learn/topic_registry.hpp"

#include <string>
#include <utility>

namespace {

class Buffer {
    std::string data_;

public:
    Buffer() = default;
    explicit Buffer(std::string s) : data_(std::move(s)) {}

    Buffer(const Buffer&) = default;
    Buffer(Buffer&&) noexcept = default;

    Buffer& operator=(Buffer other) noexcept {
        // by-value param: copy/move already done; swap cannot fail for string
        swap(other);
        return *this;
    }

    void swap(Buffer& o) noexcept { data_.swap(o.data_); }

    const std::string& get() const { return data_; }
};

void demo_basics() {
    Buffer a("hello");
    Buffer b("world");
    a = b;
    LEARN_CHECK(a.get() == "world");
    LEARN_CHECK(b.get() == "world");
}

void demo_intermediate() {
    Buffer a("old");
    Buffer b("new");
    a = std::move(b);
    LEARN_CHECK(a.get() == "new");
}

void demo_expert() {
    Buffer a("x");
    // Indirection so -Wself-assign-overloaded cannot see a = a.
    Buffer* p = &a;
    a = *p;  // self-assignment safe with copy-and-swap
    LEARN_CHECK(a.get() == "x");

    Buffer c("left");
    Buffer d("right");
    c.swap(d);
    LEARN_CHECK(c.get() == "right");
    LEARN_CHECK(d.get() == "left");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/copy_and_swap_strong_guarantee", run>;

}  // namespace
