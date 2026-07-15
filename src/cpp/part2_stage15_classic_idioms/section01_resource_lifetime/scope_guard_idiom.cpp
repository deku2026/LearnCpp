// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §1.3
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : scope_guard_idiom
// Topic id : part2/stage15/section01/scope_guard_idiom
//
// Scope guard: run cleanup at scope exit (including exception unwind).
// Motive : one-shot rollback / C-API free without a dedicated RAII type.
// Modern : GSL finally / Library Fundamentals scope_exit direction; still often
//          hand-rolled. Prefer unique_ptr + custom deleter when ownership is clear.
// Pitfall: cleanup must be noexcept-friendly; dismiss() on success paths.

#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <iostream>
#include <utility>

namespace {

template <typename F>
class ScopeGuard {
    F cleanup_;
    bool active_ = true;

public:
    explicit ScopeGuard(F f) noexcept(std::is_nothrow_move_constructible_v<F>) : cleanup_(std::move(f)) {}

    ~ScopeGuard() noexcept {
        if (active_) {
            cleanup_();  // keep cleanup non-throwing in real code
        }
    }

    void dismiss() noexcept { active_ = false; }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& o) noexcept(std::is_nothrow_move_constructible_v<F>)
        : cleanup_(std::move(o.cleanup_)), active_(o.active_) {
        o.active_ = false;
    }

    ScopeGuard& operator=(ScopeGuard&&) = delete;
};

template <typename F>
ScopeGuard(F) -> ScopeGuard<F>;

template <typename F>
[[nodiscard]] ScopeGuard<F> make_scope_exit(F f) {
    return ScopeGuard<F>{std::move(f)};
}

void demo_free_on_exit() {
    std::cout << "== free C allocation on every exit path ==\n";
    void* p = std::malloc(64);
    auto guard = make_scope_exit([&] {
        std::free(p);
        p = nullptr;
        std::cout << "  freed\n";
    });
    assert(p != nullptr);
    // ... use p; may throw ...
    (void)p;
}

void demo_rollback_and_dismiss() {
    std::cout << "== transactional rollback with dismiss ==\n";
    bool committed = false;
    int account = 100;

    try {
        account -= 30;  // tentative debit
        auto rollback = make_scope_exit([&] {
            if (!committed) {
                account += 30;
                std::cout << "  rolled back debit\n";
            }
        });

        // simulate success path
        committed = true;
        rollback.dismiss();
        std::cout << "  committed; account=" << account << '\n';
        assert(account == 70);
    } catch (...) {
        std::cout << "  exception path still rolls back via dtor\n";
        throw;
    }
}

void demo_exception_path() {
    std::cout << "== exception still runs guard ==\n";
    int flags = 0;
    try {
        auto g = make_scope_exit([&] {
            flags = 1;
            std::cout << "  guard fired after throw\n";
        });
        throw 1;
    } catch (...) {
        assert(flags == 1);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    demo_free_on_exit();
    demo_rollback_and_dismiss();
    demo_exception_path();

    std::cout << "prefer dedicated RAII (unique_ptr, lock_guard) over ad-hoc guards when possible\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/scope_guard_idiom", run>;

}  // namespace
