// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_for_cout_operator_and_swap
// Topic id : part6/d/section01/adl_for_cout_operator_and_swap
//
// Covers: ADL for operator<< and swap two-step

#include "learn/topic_registry.hpp"

#include <ostream>
#include <sstream>
#include <utility>

namespace {

namespace lib {
struct Widget {
    int id = 0;
};
std::ostream& operator<<(std::ostream& os, Widget w) {
    return os << "W" << w.id;
}
void swap(Widget& a, Widget& b) noexcept {
    int t = a.id;
    a.id = b.id;
    b.id = t;
}
}  // namespace lib

void demo_basics() {
    lib::Widget w{3};
    std::ostringstream oss;
    oss << w;  // ADL finds lib::operator<<
    LEARN_CHECK(oss.str() == "W3");
}

void demo_intermediate() {
    lib::Widget a{1}, b{2};
    using std::swap;
    swap(a, b);  // prefers lib::swap via ADL
    LEARN_CHECK(a.id == 2 && b.id == 1);
}

void demo_expert() {
    lib::Widget a{5}, b{6};
    std::swap(a, b);  // still works if move-assignable; ADL swap preferred with using
    LEARN_CHECK(a.id == 6 && b.id == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/adl_for_cout_operator_and_swap", run>;

}  // namespace
