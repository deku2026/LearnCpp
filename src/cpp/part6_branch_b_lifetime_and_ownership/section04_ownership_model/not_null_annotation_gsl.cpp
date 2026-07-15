// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : not_null_annotation_gsl
// Topic id : part6/b/section04/not_null_annotation_gsl
//
// Covers: not_null idea: APIs that reject null without GSL dep

#include "learn/topic_registry.hpp"

#include <cassert>
#include <stdexcept>

namespace {

template <class T>
class not_null {
    T ptr_;

public:
    explicit not_null(T p) : ptr_(p) {
        if (!ptr_) throw std::invalid_argument("null");
    }
    T get() const { return ptr_; }
    auto& operator*() const { return *ptr_; }
    not_null& operator=(const not_null&) = default;
};

void demo_basics() {
    int x = 5;
    not_null<int*> p{&x};
    assert(*p == 5);
}

void demo_intermediate() {
    try {
        not_null<int*> p{nullptr};
        (void)p;
        assert(false);
    } catch (const std::invalid_argument&) {
    }
}

void demo_expert() {
    int a = 1;
    int b = 2;
    not_null<int*> p{&a};
    assert(p.get() == &a);
    p = not_null<int*>{&b};
    assert(*p == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/not_null_annotation_gsl", run>;

}  // namespace
