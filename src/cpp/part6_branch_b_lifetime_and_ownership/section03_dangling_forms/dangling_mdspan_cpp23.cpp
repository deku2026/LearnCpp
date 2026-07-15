// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_mdspan_cpp23
// Topic id : part6/b/section03/dangling_mdspan_cpp23
//
// Covers: mdspan non-owning multi-dimensional view; use [] (C++23)

#include "learn/topic_registry.hpp"

#include <vector>
#if defined(__cpp_lib_mdspan)
#include <mdspan>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::mdspan<int, std::extents<std::size_t, 2, 3>> m{data.data(), 2, 3};
    LEARN_CHECK((m[0, 0] == 1));
    LEARN_CHECK((m[1, 2] == 6));
#else
    // Fallback without mdspan: row-major 2x3.
    int data[6] = {1, 2, 3, 4, 5, 6};
    auto at = [&](int r, int c) { return data[r * 3 + c]; };
    LEARN_CHECK(at(0, 0) == 1);
    LEARN_CHECK(at(1, 2) == 6);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    std::vector<int> data(6, 0);
    std::mdspan m{data.data(), std::extents<std::size_t, std::dynamic_extent, 3>{2, 3}};
    m[0, 1] = 9;
    LEARN_CHECK(data[1] == 9);
#else
    int data[6] = {};
    data[1] = 9;
    LEARN_CHECK(data[1] == 9);
#endif
}

void demo_expert() {
    // mdspan does not extend lifetime of data — owner must outlive the view.
    std::vector<int> owner{1, 2, 3, 4};
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    std::mdspan<int, std::extents<std::size_t, 2, 2>> m{owner.data()};
    LEARN_CHECK((m[1, 1] == 4));
#else
    LEARN_CHECK(owner[3] == 4);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_mdspan_cpp23", run>;

}  // namespace
