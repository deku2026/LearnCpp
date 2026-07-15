// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : has_include_and_has_cpp_attribute
// Topic id : part2/stage14/section09/has_include_and_has_cpp_attribute
//
// Companion detection tools:
//  - __has_include(<header>)           (C++17)
//  - __has_cpp_attribute(token)        (returns YYYYMM or 0)
//  - #elifdef / #elifndef / #warning   (C++23 preprocessor)
// Attributes also participate in #ifdef as "defined".

#include "learn/topic_registry.hpp"

#include <iostream>

// C++23: #elifdef / #elifndef
#ifdef __cpp_concepts
#define LEARN_HAS_CONCEPTS_MACRO 1
#elifdef __cpp_constexpr
#define LEARN_HAS_CONCEPTS_MACRO 0
#elifndef __cpp_no_such_thing
#define LEARN_HAS_CONCEPTS_MACRO 0
#endif

// #warning is C++23 — may be unsupported as directive on some front-ends;
// only emit when we detect a modern enough compiler via a known macro.
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
// #warning "stage14: C++23-class constexpr macro seen (informational)"
#endif

#if __has_include(<version>)
#include <version>
#define LEARN_HAS_VERSION_HEADER 1
#else
#define LEARN_HAS_VERSION_HEADER 0
#endif

#if __has_include(<expected>)
#define LEARN_HAS_EXPECTED_HEADER 1
#else
#define LEARN_HAS_EXPECTED_HEADER 0
#endif

#if __has_include(<mdspan>)
#define LEARN_HAS_MDSPAN_HEADER 1
#else
#define LEARN_HAS_MDSPAN_HEADER 0
#endif

#if __has_include(<generator>)
#define LEARN_HAS_GENERATOR_HEADER 1
#else
#define LEARN_HAS_GENERATOR_HEADER 0
#endif

#if __has_include(<stacktrace>)
#define LEARN_HAS_STACKTRACE_HEADER 1
#else
#define LEARN_HAS_STACKTRACE_HEADER 0
#endif

#if __has_include(<stdatomic.h>)
#define LEARN_HAS_STDATOMIC_HEADER 1
#else
#define LEARN_HAS_STDATOMIC_HEADER 0
#endif

// Nonexistent header must report 0
#if __has_include(<learncpp_no_such_header_stage14>)
#define LEARN_HAS_FAKE_HEADER 1
#else
#define LEARN_HAS_FAKE_HEADER 0
#endif

namespace {

void show_attr(const char* name, long value) {
    std::cout << "  __has_cpp_attribute(" << name << ") = " << value << '\n';
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== __has_include / __has_cpp_attribute / #elifdef ===\n";

    std::cout << "-- __has_include --\n";
    std::cout << "  <version>     = " << LEARN_HAS_VERSION_HEADER << '\n';
    std::cout << "  <expected>    = " << LEARN_HAS_EXPECTED_HEADER << '\n';
    std::cout << "  <mdspan>      = " << LEARN_HAS_MDSPAN_HEADER << '\n';
    std::cout << "  <generator>   = " << LEARN_HAS_GENERATOR_HEADER << '\n';
    std::cout << "  <stacktrace>  = " << LEARN_HAS_STACKTRACE_HEADER << '\n';
    std::cout << "  <stdatomic.h> = " << LEARN_HAS_STDATOMIC_HEADER << '\n';
    std::cout << "  <fake>        = " << LEARN_HAS_FAKE_HEADER << " (expect 0)\n";

    if (LEARN_HAS_FAKE_HEADER != 0) {
        std::cout << "ERROR: fake header should not exist\n";
        return 1;
    }

    std::cout << "-- __has_cpp_attribute (standard attributes) --\n";
    show_attr("nodiscard", __has_cpp_attribute(nodiscard));
    show_attr("maybe_unused", __has_cpp_attribute(maybe_unused));
    show_attr("fallthrough", __has_cpp_attribute(fallthrough));
    show_attr("deprecated", __has_cpp_attribute(deprecated));
    show_attr("noreturn", __has_cpp_attribute(noreturn));
    show_attr("likely", __has_cpp_attribute(likely));
    show_attr("unlikely", __has_cpp_attribute(unlikely));
    show_attr("no_unique_address", __has_cpp_attribute(no_unique_address));
    show_attr("assume", __has_cpp_attribute(assume));  // C++23

    // Attributes count as defined for #ifdef
#if __has_cpp_attribute(nodiscard)
    std::cout << "  #if __has_cpp_attribute(nodiscard) => true branch\n";
#endif

    // Use attributes when present
#if __has_cpp_attribute(maybe_unused) >= 201603L
    [[maybe_unused]] int scratch = 0;
#endif
#if __has_cpp_attribute(nodiscard) >= 201603L
    struct Prod {
        [[nodiscard]] static int make() { return 1; }
    };
    std::cout << "  nodiscard fn -> " << Prod::make() << '\n';
#endif
#if __has_cpp_attribute(assume) >= 202207L
    {
        int x = 1;
        [[assume(x == 1)]];
        std::cout << "  [[assume]] applied, x=" << x << '\n';
    }
#else
    std::cout << "  [[assume]] not available (value=" << __has_cpp_attribute(assume) << ")\n";
#endif

    std::cout << "-- #elifdef / #elifndef smoke --\n";
    std::cout << "  LEARN_HAS_CONCEPTS_MACRO chain resolved to " << LEARN_HAS_CONCEPTS_MACRO << '\n';

    std::cout << "has_include_and_has_cpp_attribute OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/has_include_and_has_cpp_attribute", run>;

}  // namespace
