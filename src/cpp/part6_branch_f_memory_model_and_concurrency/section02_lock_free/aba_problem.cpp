// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : aba_problem
// Topic id : part6/f/section02/aba_problem
//
// Covers: ABA problem concept; mitigate with versions (safe demo)

#include "learn/topic_registry.hpp"

#include <atomic>

namespace {

struct Versioned {
    std::atomic<int> value{0};
    std::atomic<int> version{0};
};

bool cas_versioned(Versioned& v, int expected_val, int new_val, int expected_ver) {
    int ver = expected_ver;
    if (!v.version.compare_exchange_strong(ver, expected_ver + 1)) return false;
    int cur = expected_val;
    if (!v.value.compare_exchange_strong(cur, new_val)) {
        // best-effort demo only
        return false;
    }
    return true;
}

void demo_basics() {
    // ABA: read A, another thread A->B->A, CAS still succeeds though state changed.
    std::atomic<int> x{1};
    int expected = 1;
    LEARN_CHECK(x.compare_exchange_strong(expected, 2));
    LEARN_CHECK(x.load() == 2);
}

void demo_intermediate() {
    Versioned v;
    v.value.store(1);
    v.version.store(0);
    LEARN_CHECK(v.value.load() == 1);
    LEARN_CHECK(cas_versioned(v, 1, 2, 0));
    LEARN_CHECK(v.value.load() == 2);
    LEARN_CHECK(v.version.load() == 1);
    LEARN_CHECK(!cas_versioned(v, 2, 3, 0));  // stale version
}

void demo_expert() {
    // Tagged pointers / version counters help detect ABA; full lock-free stacks need care.
    std::atomic<int> tag{0};
    int t = tag.fetch_add(1);
    LEARN_CHECK(t == 0);
    LEARN_CHECK(tag.load() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/aba_problem", run>;

}  // namespace
