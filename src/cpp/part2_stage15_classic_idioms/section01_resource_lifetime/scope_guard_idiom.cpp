// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : scope_guard_idiom
// Topic id : part2/stage15/section01/scope_guard_idiom
//
// Covers: scope guard cleanup on scope exit

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

template <class F>
class ScopeGuard {
    F f_;
    bool active_ = true;

public:
    explicit ScopeGuard(F f) : f_(std::move(f)) {}
    ~ScopeGuard() {
        if (active_) {
            f_();
        }
    }
    void dismiss() { active_ = false; }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&& o) noexcept : f_(std::move(o.f_)), active_(o.active_) { o.active_ = false; }
};
template <class F>
ScopeGuard(F) -> ScopeGuard<F>;

void demo_basics() {
    int n = 0;
    {
        ScopeGuard g([&] { n = 1; });
        LEARN_CHECK(n == 0);
    }
    LEARN_CHECK(n == 1);
}

void demo_intermediate() {
    int n = 0;
    {
        ScopeGuard g([&] { n = 1; });
        g.dismiss();
    }
    LEARN_CHECK(n == 0);
}

void demo_expert() {
    int n = 0;
    try {
        ScopeGuard g([&] { n = 2; });
        throw 1;
    } catch (...) {
    }
    LEARN_CHECK(n == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/scope_guard_idiom", run>;

}  // namespace
