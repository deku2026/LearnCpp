// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : owner_annotation_gsl
// Topic id : part6/b/section04/owner_annotation_gsl
//
// Covers: GSL owner<T*> idea: mark owning raw pointers (without GSL dep)

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

// Stand-in for gsl::owner<T*>
template <class T>
using owner = T;

struct FileLike {
    owner<int*> buf = nullptr;
    explicit FileLike(int v) : buf(new int(v)) {}
    ~FileLike() { delete buf; }
    FileLike(const FileLike&) = delete;
    FileLike& operator=(const FileLike&) = delete;
};

void demo_basics() {
    FileLike f{3};
    LEARN_CHECK(f.buf != nullptr);
    LEARN_CHECK(*f.buf == 3);
}

void demo_intermediate() {
    // owner annotation documents who must delete; prefer unique_ptr in new code.
    owner<int*> p = new int(8);
    LEARN_CHECK(*p == 8);
    delete p;
}

void demo_expert() {
    FileLike f{1};
    LEARN_CHECK(*f.buf == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/owner_annotation_gsl", run>;

}  // namespace
