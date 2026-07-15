// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : copy_and_swap_idiom
// Topic id : part2/stage15/section01/copy_and_swap_idiom
//
// Covers: copy-and-swap assignment strong guarantee

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <utility>

namespace {

class Buffer {
    int* p_;
    std::size_t n_;

public:
    Buffer(std::size_t n, int v) : p_(new int[n]), n_(n) { std::fill(p_, p_ + n_, v); }
    ~Buffer() { delete[] p_; }
    Buffer(const Buffer& o) : p_(new int[o.n_]), n_(o.n_) { std::copy(o.p_, o.p_ + n_, p_); }
    Buffer(Buffer&& o) noexcept : p_(o.p_), n_(o.n_) {
        o.p_ = nullptr;
        o.n_ = 0;
    }
    friend void swap(Buffer& a, Buffer& b) noexcept {
        using std::swap;
        swap(a.p_, b.p_);
        swap(a.n_, b.n_);
    }
    Buffer& operator=(Buffer o) noexcept {
        swap(*this, o);
        return *this;
    }
    int at(std::size_t i) const { return p_[i]; }
    std::size_t size() const { return n_; }
};

void demo_basics() {
    Buffer a(2, 1);
    Buffer b(2, 9);
    a = b;
    LEARN_CHECK(a.at(0) == 9 && a.at(1) == 9);
}

void demo_intermediate() {
    Buffer a(3, 2);
    Buffer b = a;
    LEARN_CHECK(b.size() == 3 && b.at(2) == 2);
}

void demo_expert() {
    Buffer a(1, 5);
    Buffer b(1, 7);
    a = std::move(b);
    LEARN_CHECK(a.at(0) == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/copy_and_swap_idiom", run>;

}  // namespace
