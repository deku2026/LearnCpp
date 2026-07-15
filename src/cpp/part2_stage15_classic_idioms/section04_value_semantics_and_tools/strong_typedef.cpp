// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : strong_typedef
// Topic id : part2/stage15/section04/strong_typedef
//
// Covers: strong typedef / opaque alias

#include "learn/topic_registry.hpp"

#include <string>

namespace {

template <class Tag, class T>
struct Strong {
    T value;
    explicit Strong(T v) : value(std::move(v)) {}
};

struct UserIdTag {};
struct GroupIdTag {};
using UserId = Strong<UserIdTag, int>;
using GroupId = Strong<GroupIdTag, int>;

int load_user(UserId id) {
    return id.value;
}

void demo_basics() {
    UserId u{7};
    LEARN_CHECK(load_user(u) == 7);
}

void demo_intermediate() {
    UserId u{1};
    GroupId g{1};
    LEARN_CHECK(u.value == g.value);
    // load_user(g); // would not compile — different types
}

void demo_expert() {
    struct MeterTag {};
    using Meter = Strong<MeterTag, double>;
    Meter m{3.5};
    LEARN_CHECK(m.value == 3.5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/strong_typedef", run>;

}  // namespace
