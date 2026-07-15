// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §3.4
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : cpo_customization_point
// Topic id : part2/stage15/section03/cpo_customization_point
//
// CPO (Customization Point Object): reliable user extension points for libraries.
// Motive : let users customize begin/size/serialize via members or ADL without
//          specializing fragile function templates.
// Modern : std::ranges::begin/end/size are CPOs; niebloids.
// Pitfall: do not open-std specialize; prefer member or same-namespace free function.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace {

// ---------- teaching-size CPO: mylib::size ----------
// Simplified real-world shape:
//   1) member .size() wins
//   2) otherwise ADL free function size(t)
// Poisoned unqualified size in the CPO's associated detail scope blocks hijacks.
namespace mylib {
namespace detail {

void size() = delete;

template <typename T>
concept HasMemberSize = requires(T&& t) {
    { std::forward<T>(t).size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept HasAdlSize = requires(T&& t) {
    { size(std::forward<T>(t)) } -> std::convertible_to<std::size_t>;
};

struct size_fn {
    template <HasMemberSize T>
    constexpr std::size_t operator()(T&& t) const noexcept(noexcept(std::forward<T>(t).size())) {
        return static_cast<std::size_t>(std::forward<T>(t).size());
    }

    template <typename T>
        requires(!HasMemberSize<T> && HasAdlSize<T>)
    constexpr std::size_t operator()(T&& t) const {
        using detail::size;  // bring poison into overload set
        return static_cast<std::size_t>(size(std::forward<T>(t)));
    }
};

}  // namespace detail

inline constexpr detail::size_fn size{};

}  // namespace mylib

// User type customized via member
struct MemberSized {
    int n = 0;
    std::size_t size() const noexcept { return static_cast<std::size_t>(n); }
};

// User type customized via ADL free function (no member size)
namespace userlib {

struct AdlSized {
    int items = 0;
};

std::size_t size(const AdlSized& a) noexcept {
    return static_cast<std::size_t>(a.items);
}

}  // namespace userlib

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== CPO mylib::size ==\n";
    std::vector<int> v{1, 2, 3, 4};
    assert(mylib::size(v) == 4);
    std::cout << "  vector size via member: " << mylib::size(v) << '\n';

    MemberSized m{.n = 7};
    assert(mylib::size(m) == 7);
    std::cout << "  MemberSized: " << mylib::size(m) << '\n';

    userlib::AdlSized a{.items = 11};
    assert(mylib::size(a) == 11);
    std::cout << "  AdlSized via ADL: " << mylib::size(a) << '\n';

    std::cout << "== standard ranges CPO ==\n";
    assert(std::ranges::size(v) == 4);
    auto first = std::ranges::begin(v);
    assert(*first == 1);
    std::cout << "  ranges::size/begin behave as CPOs; *begin=" << *first << '\n';

    std::cout << "library authors: ship CPOs; app code: just call ranges::*\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/cpo_customization_point", run>;

}  // namespace
