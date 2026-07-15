// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §4.1
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : non_copyable_non_movable_via_delete
// Topic id : part2/stage15/section04/non_copyable_non_movable_via_delete
//
// =delete special members to encode ownership in the type system.
// Motive : prevent accidental copy of unique resources / pinned objects.
// Modern : prefer =delete over private-and-undefined (C++98); Rule of 0 still first.
// Pitfall: deleting copy without considering move; deleted functions still participate
//          in overload resolution and can yield hard errors.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

namespace {

// Move-only: unique ownership (unique_ptr style).
class MoveOnlyHandle {
    int id_ = 0;

public:
    explicit MoveOnlyHandle(int id) : id_(id) {}

    MoveOnlyHandle(const MoveOnlyHandle&) = delete;
    MoveOnlyHandle& operator=(const MoveOnlyHandle&) = delete;

    MoveOnlyHandle(MoveOnlyHandle&& o) noexcept : id_(std::exchange(o.id_, 0)) {}
    MoveOnlyHandle& operator=(MoveOnlyHandle&& o) noexcept {
        if (this != &o) {
            id_ = std::exchange(o.id_, 0);
        }
        return *this;
    }

    int id() const noexcept { return id_; }
};

// Neither copy nor move: mutex-like / scoped binding.
class Pinned {
    int token_ = 0;

public:
    explicit Pinned(int token) : token_(token) {}

    Pinned(const Pinned&) = delete;
    Pinned& operator=(const Pinned&) = delete;
    // Declaring deleted copy suppresses implicit move generation.

    int token() const noexcept { return token_; }
};

// Delete selected overloads (not only special members).
void take_int(int x) {
    std::cout << "  take_int " << x << '\n';
}
void take_int(bool) = delete;  // block bool promotions by mistake
void take_int(double) = delete;

// Mixin-style non-copyable base (C++98 heritage; prefer =delete on the type itself).
class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class Service : private NonCopyable {
    std::unique_ptr<int> state_ = std::make_unique<int>(1);

public:
    int get() const { return *state_; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== move-only handle ==\n";
    MoveOnlyHandle a{42};
    MoveOnlyHandle b = std::move(a);
    assert(b.id() == 42);
    assert(a.id() == 0);
    // MoveOnlyHandle c = b; // ill-formed: copy deleted
    std::cout << "  moved id=" << b.id() << '\n';

    std::cout << "== pinned (no copy, no move) ==\n";
    Pinned p{7};
    assert(p.token() == 7);
    // auto q = std::move(p); // ill-formed
    std::cout << "  token=" << p.token() << '\n';

    std::cout << "== deleted overload protects API ==\n";
    take_int(3);
    // take_int(true); // deleted
    // take_int(1.0);  // deleted

    Service s;
    assert(s.get() == 1);
    std::cout << "  NonCopyable mixin service ok\n";

    std::cout << "only restrict copy/move when ownership truly requires it\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/non_copyable_non_movable_via_delete", run>;

}  // namespace
