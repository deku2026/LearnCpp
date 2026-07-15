// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_library_feature_macros
// Topic id : part2/stage14/section09/cpp_library_feature_macros
//
// Library feature-test macros (__cpp_lib_*) are NOT predefined — pull them
// via #include <version> (or the header that provides the feature). Dump a
// C++17→23 matrix and gate a real API call on the macro value.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <version>

// Optional feature headers for gated demos
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#include <string>
#endif
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#endif
#include <optional>
#include <string>
#include <vector>

namespace {

#define LEARN_SHOW_LIB(name)           \
    do {                               \
        std::cout << "  " #name " = "; \
        if (name)                      \
            std::cout << name;         \
        else                           \
            std::cout << "0";          \
        std::cout << '\n';             \
    } while (0)

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== Library feature-test macros (__cpp_lib_*) via <version> ===\n";

    std::cout << "-- C++17 anchors --\n";
#ifdef __cpp_lib_optional
    LEARN_SHOW_LIB(__cpp_lib_optional);
#endif
#ifdef __cpp_lib_variant
    LEARN_SHOW_LIB(__cpp_lib_variant);
#endif
#ifdef __cpp_lib_any
    LEARN_SHOW_LIB(__cpp_lib_any);
#endif
#ifdef __cpp_lib_string_view
    LEARN_SHOW_LIB(__cpp_lib_string_view);
#endif
#ifdef __cpp_lib_filesystem
    LEARN_SHOW_LIB(__cpp_lib_filesystem);
#endif
#ifdef __cpp_lib_execution
    LEARN_SHOW_LIB(__cpp_lib_execution);
#endif

    std::cout << "-- C++20 anchors --\n";
#ifdef __cpp_lib_ranges
    LEARN_SHOW_LIB(__cpp_lib_ranges);
#endif
#ifdef __cpp_lib_format
    LEARN_SHOW_LIB(__cpp_lib_format);
#endif
#ifdef __cpp_lib_span
    LEARN_SHOW_LIB(__cpp_lib_span);
#endif
#ifdef __cpp_lib_concepts
    LEARN_SHOW_LIB(__cpp_lib_concepts);
#endif
#ifdef __cpp_lib_jthread
    LEARN_SHOW_LIB(__cpp_lib_jthread);
#endif
#ifdef __cpp_lib_bitops
    LEARN_SHOW_LIB(__cpp_lib_bitops);
#endif
#ifdef __cpp_lib_bit_cast
    LEARN_SHOW_LIB(__cpp_lib_bit_cast);
#endif
#ifdef __cpp_lib_source_location
    LEARN_SHOW_LIB(__cpp_lib_source_location);
#endif
#ifdef __cpp_lib_three_way_comparison
    LEARN_SHOW_LIB(__cpp_lib_three_way_comparison);
#endif

    std::cout << "-- C++23 signature library macros --\n";
#ifdef __cpp_lib_expected
    LEARN_SHOW_LIB(__cpp_lib_expected);
#else
    std::cout << "  __cpp_lib_expected = <undefined>\n";
#endif
#ifdef __cpp_lib_print
    LEARN_SHOW_LIB(__cpp_lib_print);
#else
    std::cout << "  __cpp_lib_print = <undefined>\n";
#endif
#ifdef __cpp_lib_mdspan
    LEARN_SHOW_LIB(__cpp_lib_mdspan);
#else
    std::cout << "  __cpp_lib_mdspan = <undefined>\n";
#endif
#ifdef __cpp_lib_generator
    LEARN_SHOW_LIB(__cpp_lib_generator);
#else
    std::cout << "  __cpp_lib_generator = <undefined>\n";
#endif
#ifdef __cpp_lib_ranges_to_container
    LEARN_SHOW_LIB(__cpp_lib_ranges_to_container);
#else
    std::cout << "  __cpp_lib_ranges_to_container = <undefined>\n";
#endif
#ifdef __cpp_lib_ranges_fold
    LEARN_SHOW_LIB(__cpp_lib_ranges_fold);
#else
    std::cout << "  __cpp_lib_ranges_fold = <undefined>\n";
#endif
#ifdef __cpp_lib_flat_map
    LEARN_SHOW_LIB(__cpp_lib_flat_map);
#else
    std::cout << "  __cpp_lib_flat_map = <undefined>\n";
#endif
#ifdef __cpp_lib_move_only_function
    LEARN_SHOW_LIB(__cpp_lib_move_only_function);
#else
    std::cout << "  __cpp_lib_move_only_function = <undefined>\n";
#endif
#ifdef __cpp_lib_stacktrace
    LEARN_SHOW_LIB(__cpp_lib_stacktrace);
#else
    std::cout << "  __cpp_lib_stacktrace = <undefined>\n";
#endif
#ifdef __cpp_lib_allocate_at_least
    LEARN_SHOW_LIB(__cpp_lib_allocate_at_least);
#else
    std::cout << "  __cpp_lib_allocate_at_least = <undefined>\n";
#endif
#ifdef __cpp_lib_byteswap
    LEARN_SHOW_LIB(__cpp_lib_byteswap);
#else
    std::cout << "  __cpp_lib_byteswap = <undefined>\n";
#endif
#ifdef __cpp_lib_to_underlying
    LEARN_SHOW_LIB(__cpp_lib_to_underlying);
#else
    std::cout << "  __cpp_lib_to_underlying = <undefined>\n";
#endif
#ifdef __cpp_lib_modules
    LEARN_SHOW_LIB(__cpp_lib_modules);
#else
    std::cout << "  __cpp_lib_modules = <undefined>\n";
#endif
#ifdef __cpp_lib_bind_back
    LEARN_SHOW_LIB(__cpp_lib_bind_back);
#else
    std::cout << "  __cpp_lib_bind_back = <undefined>\n";
#endif
#ifdef __cpp_lib_forward_like
    LEARN_SHOW_LIB(__cpp_lib_forward_like);
#else
    std::cout << "  __cpp_lib_forward_like = <undefined>\n";
#endif
#ifdef __cpp_lib_out_ptr
    LEARN_SHOW_LIB(__cpp_lib_out_ptr);
#else
    std::cout << "  __cpp_lib_out_ptr = <undefined>\n";
#endif

    // Gated real use
    std::cout << "-- gated API demos --\n";
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    {
        std::expected<int, std::string> e = 42;
        std::cout << "  expected OK *e=" << *e << '\n';
    }
#else
    std::cout << "  expected gated OFF\n";
#endif

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    std::println("  print gated ON value={}", 42);
#else
    std::cout << "  print gated OFF\n";
#endif

    // Always-available baseline so the topic is never empty of runtime effect
    std::optional<int> o = 7;
    std::cout << "  baseline optional=" << *o << " vector_cap_demo=" << std::vector<int>{1, 2, 3}.size() << '\n';

    std::cout << "cpp_library_feature_macros OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/cpp_library_feature_macros", run>;

}  // namespace
