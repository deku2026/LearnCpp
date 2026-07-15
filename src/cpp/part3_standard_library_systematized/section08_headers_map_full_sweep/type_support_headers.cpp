// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : type_support_headers
// Topic id : part3/section08/type_support_headers
//
// Covers: headers map: type_traits cstddef cstdint limits typeinfo

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <typeinfo>

namespace {

void demo_basics() {
    static_assert(sizeof(std::int32_t) == 4);
    LEARN_CHECK(typeid(int).name() != nullptr);
}

void demo_intermediate() {
    LEARN_CHECK(std::is_integral_v<int>);
    LEARN_CHECK(std::numeric_limits<int>::is_specialized);
}

void demo_expert() {
    LEARN_CHECK(sizeof(std::size_t) >= 4);
    LEARN_CHECK(alignof(std::max_align_t) >= 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/type_support_headers", run>;

}  // namespace
