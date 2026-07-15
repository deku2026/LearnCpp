// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part3/section06/constexpr_unique_ptr_cpp23
//
// Covers: constexpr unique_ptr improvements C++23

#include "learn/topic_registry.hpp"

#include <memory>
#include <version>

namespace {

void demo_basics() {
    auto p = std::make_unique<int>(3);
    LEARN_CHECK(*p == 3);
}

void demo_intermediate() {
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    constexpr auto v = [] {
        std::unique_ptr<int> p{new int{5}};
        return *p;
    }();
    LEARN_CHECK(v == 5);
#else
    auto p = std::unique_ptr<int>{new int{5}};
    LEARN_CHECK(*p == 5);
#endif
}

void demo_expert() {
    std::unique_ptr<int> a = std::make_unique<int>(1);
    std::unique_ptr<int> b = std::move(a);
    LEARN_CHECK(!a);
    LEARN_CHECK(b && *b == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/constexpr_unique_ptr_cpp23", run>;

}  // namespace
