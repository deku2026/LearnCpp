// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : unique_ptr_custom_deleter
// Topic id : part2/stage05/section04/unique_ptr_custom_deleter
//
// Covers: custom deleters for non-new resources (RAII one-liner)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

struct Handle {
    static int open_count;
    int id = 0;
};
int Handle::open_count = 0;

Handle* open_handle(int id) {
    auto* h = new Handle{id};
    ++Handle::open_count;
    return h;
}

void close_handle(Handle* h) {
    if (h) {
        --Handle::open_count;
        delete h;
    }
}

void demo_basics() {
    Handle::open_count = 0;
    {
        auto closer = [](Handle* h) { close_handle(h); };
        std::unique_ptr<Handle, decltype(closer)> hp(open_handle(1), closer);
        assert(hp != nullptr);
        assert(hp->id == 1);
        assert(Handle::open_count == 1);
    }
    assert(Handle::open_count == 0);
}

void demo_intermediate() {
    Handle::open_count = 0;
    using HPtr = std::unique_ptr<Handle, void (*)(Handle*)>;
    HPtr a(open_handle(2), &close_handle);
    HPtr b = std::move(a);
    assert(a == nullptr);
    assert(b->id == 2);
    b.reset();
    assert(Handle::open_count == 0);
}

void demo_expert() {
    // Stateful function-object deleter (empty capture stays small).
    struct Closer {
        int* closes;
        void operator()(Handle* h) const {
            if (closes) {
                ++(*closes);
            }
            close_handle(h);
        }
    };

    Handle::open_count = 0;
    int closes = 0;
    {
        std::unique_ptr<Handle, Closer> hp(open_handle(3), Closer{&closes});
        assert(hp->id == 3);
        assert(Handle::open_count == 1);
    }
    assert(closes == 1);
    assert(Handle::open_count == 0);

    // Array form with default_delete is the common non-scalar case.
    std::unique_ptr<int[]> arr(new int[3]{1, 2, 3});
    assert(arr[2] == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/unique_ptr_custom_deleter", run>;

}  // namespace
