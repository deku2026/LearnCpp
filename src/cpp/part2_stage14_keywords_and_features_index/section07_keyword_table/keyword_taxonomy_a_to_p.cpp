// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section07_keyword_table
// Item     : keyword_taxonomy_a_to_p
// Topic id : part2/stage14/section07/keyword_taxonomy_a_to_p
//
// Covers: keyword taxonomy samples A-P

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

void demo_basics() {
    // types / cv / storage samples
    bool b = true;
    char c = 'a';
    int i = 0;
    const int ci = 1;
    volatile int vi = 2;
    static int si = 3;
    (void)si;
    LEARN_CHECK(b && c == 'a' && i == 0 && ci == 1 && vi == 2);
}

void demo_intermediate() {
    // class / control / exception keywords in miniature
    struct Base {
        virtual int f() { return 1; }
        virtual ~Base() = default;
    };
    struct Der : Base {
        int f() override { return 2; }
    };
    Der d;
    Base& r = d;
    LEARN_CHECK(r.f() == 2);
    try {
        throw 1;
    } catch (...) {
        LEARN_CHECK(true);
    }
}

void demo_expert() {
    // templates / concepts-ish / operators as keywords
    LEARN_CHECK((true and false) == false);
    LEARN_CHECK((true or false) == true);
    LEARN_CHECK((not false) == true);
    using T = int;
    static_assert(std::is_same_v<T, int>);
    enum class E { A };
    LEARN_CHECK(static_cast<int>(E::A) == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section07/keyword_taxonomy_a_to_p", run>;

}  // namespace
