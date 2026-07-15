// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_default_noexcept
// Topic id : part2/stage09/section07/destructor_default_noexcept
//
// Covers: destructors are noexcept by default unless declared otherwise

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct DefaultDtor {
    int x = 0;
};

struct ExplicitNoexcept {
    ~ExplicitNoexcept() noexcept {}
};

struct Member {
    ~Member() noexcept {}
};

struct Composite {
    Member m;
};

void demo_basics() {
    static_assert(std::is_nothrow_destructible_v<DefaultDtor>);
    static_assert(std::is_nothrow_destructible_v<ExplicitNoexcept>);
    assert(std::is_nothrow_destructible_v<DefaultDtor>);
}

void demo_intermediate() {
    static_assert(std::is_nothrow_destructible_v<Composite>);
    DefaultDtor d;
    assert(d.x == 0);
}

void demo_expert() {
    // User-declared ~T() noexcept(false) is possible but dangerous; we avoid it.
    struct Careful {
        ~Careful() noexcept = default;
    };
    static_assert(std::is_nothrow_destructible_v<Careful>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section07/destructor_default_noexcept", run>;

}  // namespace
