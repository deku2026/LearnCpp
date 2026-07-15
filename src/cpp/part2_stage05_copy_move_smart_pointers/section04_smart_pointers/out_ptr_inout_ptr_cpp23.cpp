// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : out_ptr_inout_ptr_cpp23
// Topic id : part2/stage05/section04/out_ptr_inout_ptr_cpp23
//
// Covers: std::out_ptr / inout_ptr adapt smart pointers to C T** APIs

#include "learn/topic_registry.hpp"

#include <memory>
#include <version>

namespace {

// Simulated C API: writes a new allocation into *out.
int c_create(int** out) {
    if (!out) {
        return -1;
    }
    *out = new int(42);
    return 0;
}

// Simulated C API: may free old and write a new pointer.
int c_replace(int** io_pp) {
    if (!io_pp) {
        return -1;
    }
    delete *io_pp;
    *io_pp = new int(99);
    return 0;
}

void demo_basics() {
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    std::unique_ptr<int> p;
    const int rc = c_create(std::out_ptr(p));
    LEARN_CHECK(rc == 0);
    LEARN_CHECK(p != nullptr);
    LEARN_CHECK(*p == 42);
#else
    // Manual adaptation without out_ptr.
    int* raw = nullptr;
    LEARN_CHECK(c_create(&raw) == 0);
    std::unique_ptr<int> p(raw);
    LEARN_CHECK(*p == 42);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    std::unique_ptr<int> p = std::make_unique<int>(1);
    const int rc = c_replace(std::inout_ptr(p));
    LEARN_CHECK(rc == 0);
    LEARN_CHECK(p != nullptr);
    LEARN_CHECK(*p == 99);
#else
    std::unique_ptr<int> p = std::make_unique<int>(1);
    int* raw = p.release();
    LEARN_CHECK(c_replace(&raw) == 0);
    p.reset(raw);
    LEARN_CHECK(*p == 99);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    // shared_ptr + out_ptr requires an explicit deleter (standard rule).
    std::shared_ptr<int> sp;
    LEARN_CHECK(c_create(std::out_ptr(sp, std::default_delete<int>{})) == 0);
    LEARN_CHECK(sp.use_count() == 1);
    LEARN_CHECK(*sp == 42);

    // out_ptr replaces contents; previous value is released on success path.
    std::unique_ptr<int> u = std::make_unique<int>(7);
    LEARN_CHECK(c_create(std::out_ptr(u)) == 0);
    LEARN_CHECK(*u == 42);
#else
    std::shared_ptr<int> sp;
    int* raw = nullptr;
    LEARN_CHECK(c_create(&raw) == 0);
    sp.reset(raw);
    LEARN_CHECK(*sp == 42);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/out_ptr_inout_ptr_cpp23", run>;

}  // namespace
