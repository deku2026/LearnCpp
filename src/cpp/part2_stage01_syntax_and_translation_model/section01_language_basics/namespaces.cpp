// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : namespaces
// Topic id : part2/stage01/section01/namespaces
//
// Covers: nested namespaces, using-directive vs using-declaration, light ADL, anonymous namespace

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace lib {
inline constexpr int kVersion = 1;

namespace detail {
inline constexpr int secret = 99;
}  // namespace detail

struct Widget {
    int id = 0;
};

// Associated namespace for ADL
int tag(const Widget& w) {
    return w.id;
}

namespace v2 {
inline constexpr int kVersion = 2;
}  // namespace v2
}  // namespace lib

namespace app::util {
inline int scale(int x) {
    return x * 2;
}
}  // namespace app::util

namespace {

// Unnamed namespace: internal linkage for this TU
constexpr int tu_only = 7;

void demo_basics() {
    assert(lib::kVersion == 1);
    assert(lib::detail::secret == 99);
    assert(app::util::scale(3) == 6);
    assert(tu_only == 7);
}

void demo_intermediate() {
    // using-declaration: brings one name into scope
    using app::util::scale;
    assert(scale(4) == 8);

    // using-directive: brings all names (use sparingly)
    {
        using namespace lib::v2;
        assert(kVersion == 2);
    }
    assert(lib::kVersion == 1);

    lib::Widget w{42};
    assert(w.id == 42);
}

void demo_expert() {
    lib::Widget w{11};
    // ADL: unqualified tag() finds lib::tag via associated namespace of Widget
    assert(tag(w) == 11);
    assert(lib::tag(w) == 11);

    // Nested namespace alias
    namespace L = lib::detail;
    assert(L::secret == 99);

    // Argument-dependent lookup with std::swap pattern (light intro)
    int a = 1;
    int b = 2;
    using std::swap;
    swap(a, b);
    assert(a == 2 && b == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/namespaces", run>;

}  // namespace
