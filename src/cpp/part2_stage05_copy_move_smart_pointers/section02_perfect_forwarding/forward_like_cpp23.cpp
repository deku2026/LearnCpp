// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : forward_like_cpp23
// Topic id : part2/stage05/section02/forward_like_cpp23
//
// Covers: std::forward_like (C++23) forwards member with owner's value category

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>
#include <version>

namespace {

struct Scheduler {
    int copies = 0;
    int moves = 0;

    bool submit(const std::string&) {
        ++copies;
        return true;
    }
    bool submit(std::string&&) {
        ++moves;
        return true;
    }
};

void demo_basics() {
#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    std::string m = "msg";
    std::string lvalue_owner;
    auto&& as_lvalue = std::forward_like<decltype(lvalue_owner)&>(m);
    static_assert(std::is_lvalue_reference_v<decltype(as_lvalue)>);

    auto&& as_rvalue = std::forward_like<std::string&&>(m);
    static_assert(std::is_rvalue_reference_v<decltype(as_rvalue)>);
#else
    // Feature unavailable: still exercise category-preserving intent via forward.
    std::string m = "msg";
    auto&& x = std::forward<std::string&>(m);
    static_assert(std::is_lvalue_reference_v<decltype(x)>);
#endif
}

void demo_intermediate() {
    Scheduler scheduler;

#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    auto callback = [m = std::string("msg"), &scheduler](this auto&& self) -> bool {
        return scheduler.submit(std::forward_like<decltype(self)>(m));
    };

    assert(callback());  // lvalue closure -> copy submit
    assert(scheduler.copies == 1);
    assert(scheduler.moves == 0);

    assert(std::move(callback)());  // rvalue closure -> move submit
    assert(scheduler.moves == 1);
#else
    // Fallback without forward_like / deducing this: manual branches.
    std::string m = "msg";
    assert(scheduler.submit(m));
    assert(scheduler.copies == 1);
    assert(scheduler.submit(std::move(m)));
    assert(scheduler.moves == 1);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    struct Bag {
        std::string item{"x"};
    };
    Bag bag;
    auto take = [](auto&& owner) { return std::string(std::forward_like<decltype(owner)>(owner.item)); };
    std::string a = take(bag);
    std::string b = take(std::move(bag));
    assert(a == "x");
    assert(b == "x");
#else
    struct Bag {
        std::string item{"x"};
    };
    Bag bag;
    std::string a = bag.item;
    std::string b = std::move(bag.item);
    assert(a == "x");
    assert(b == "x");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/forward_like_cpp23", run>;

}  // namespace
