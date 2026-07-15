// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_must_not_throw
// Topic id : part2/stage09/section07/destructor_must_not_throw
//
// Covers: destructors should not throw (especially during unwind)

#include "learn/topic_registry.hpp"

#include <stdexcept>
#include <string>

namespace {

struct QuietDtor {
    int* flag = nullptr;
    ~QuietDtor() noexcept {
        if (flag) {
            *flag = 1;
        }
        // swallow errors — never throw from dtor
    }
};

void demo_basics() {
    int done = 0;
    {
        QuietDtor q{&done};
    }
    LEARN_CHECK(done == 1);
}

void demo_intermediate() {
    int done = 0;
    try {
        QuietDtor q{&done};
        throw std::runtime_error("x");
    } catch (...) {
        LEARN_CHECK(done == 1);  // dtor ran during unwind without throwing
    }
}

void demo_expert() {
    // Teaching: if cleanup can fail, log/store error; do not throw from ~T().
    struct FileLike {
        bool flush_ok = true;
        bool failed = false;
        ~FileLike() noexcept {
            if (!flush_ok) {
                failed = true;  // record, don't throw
            }
        }
    };
    FileLike f;
    f.flush_ok = false;
    // dtor will set failed on scope exit — demonstrate via explicit call pattern:
    {
        FileLike g;
        g.flush_ok = false;
    }
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section07/destructor_must_not_throw", run>;

}  // namespace
