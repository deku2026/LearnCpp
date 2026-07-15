// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : detection_idiom
// Topic id : part2/stage15/section03/detection_idiom
//
// Covers: void_t detection idiom

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

template <class, class = void>
struct has_size : std::false_type {};

template <class T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

struct With {
    int size() const { return 3; }
};
struct Without {};

void demo_basics() {
    static_assert(has_size<With>::value);
    static_assert(!has_size<Without>::value);
    LEARN_CHECK(has_size<With>::value);
}

void demo_intermediate() {
    With w;
    if constexpr (has_size<With>::value) {
        LEARN_CHECK(w.size() == 3);
    }
}

void demo_expert() {
#if defined(__cpp_concepts)
    LEARN_CHECK(true);
#else
    LEARN_CHECK(true);
#endif
    static_assert(std::is_same_v<std::void_t<int>, void>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/detection_idiom", run>;

}  // namespace
