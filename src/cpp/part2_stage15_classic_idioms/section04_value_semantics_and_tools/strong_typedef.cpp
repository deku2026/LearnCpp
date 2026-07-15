// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §4.2
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : strong_typedef
// Topic id : part2/stage15/section04/strong_typedef
//
// Strong typedef: distinct types over the same underlying representation.
// Motive : stop mixing UserId/ProductId/meters/seconds at compile time.
// Modern : tag-parameter wrapper; libraries like NamedType; std has no official one.
// Pitfall: over-engineering local ints; apply at API boundaries and unit-sensitive code.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <compare>
#include <iostream>
#include <string>
#include <utility>

namespace {

template <typename T, typename Tag>
class Strong {
    T value_{};

public:
    constexpr Strong() = default;
    constexpr explicit Strong(T v) : value_(std::move(v)) {}

    constexpr const T& get() const& noexcept { return value_; }
    constexpr T& get() & noexcept { return value_; }
    constexpr T&& get() && noexcept { return std::move(value_); }

    constexpr explicit operator const T&() const noexcept { return value_; }

    friend constexpr bool operator==(const Strong&, const Strong&) = default;
    friend constexpr auto operator<=>(const Strong&, const Strong&) = default;
};

// Phantom tags make UserId and ProductId different types.
using UserId = Strong<int, struct UserIdTag>;
using ProductId = Strong<int, struct ProductIdTag>;
using Meters = Strong<double, struct MetersTag>;
using Seconds = Strong<double, struct SecondsTag>;

void process_user(UserId id) {
    std::cout << "  process user #" << id.get() << '\n';
}

void process_product(ProductId id) {
    std::cout << "  process product #" << id.get() << '\n';
}

// Optional free ops for arithmetic units (same strong type only).
Meters operator+(Meters a, Meters b) {
    return Meters{a.get() + b.get()};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== distinct ids ==\n";
    UserId uid{42};
    ProductId pid{42};  // same underlying 42, different type
    process_user(uid);
    process_product(pid);
    // process_user(pid); // ill-formed
    // process_user(42);  // ill-formed: explicit ctor

    assert(uid.get() == pid.get());
    assert(uid == UserId{42});
    assert(uid != UserId{0});

    std::cout << "== units ==\n";
    Meters dist = Meters{3.0} + Meters{1.5};
    assert(dist.get() == 4.5);
    Seconds t{2.0};
    // auto nonsense = dist + t; // ill-formed if only Meters+Meters provided
    std::cout << "  meters=" << dist.get() << " seconds=" << t.get() << '\n';

    std::cout << "use at boundaries; do not wrap every local loop index\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/strong_typedef", run>;

}  // namespace
