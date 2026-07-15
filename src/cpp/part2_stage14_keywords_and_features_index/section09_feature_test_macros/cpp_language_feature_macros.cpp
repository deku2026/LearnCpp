// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_language_feature_macros
// Topic id : part2/stage14/section09/cpp_language_feature_macros
//
// Language feature-test macros (__cpp_*) are predefined in every TU — no
// #include required. Values are YYYYMM of the paper landing (and bump on
// major revisions). Dump a representative matrix C++11→23 and exercise
// conditional compilation on a few anchors.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <utility>

namespace {

#define LEARN_SHOW_LANG(name)                \
    do {                                     \
        std::cout << "  " << #name << " = "; \
        if (name != 0)                       \
            std::cout << name;               \
        else                                 \
            std::cout << "<undefined/0>";    \
        std::cout << '\n';                   \
    } while (0)

// Safe read: if macro is undefined, substitute 0 via #ifdef wrapper
#define LEARN_LANG_VAL(name) (name)

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== Language feature-test macros (__cpp_*) ===\n";
    std::cout << "__cplusplus = " << __cplusplus << '\n';

    std::cout << "-- C++11 era --\n";
#ifdef __cpp_alias_templates
    LEARN_SHOW_LANG(__cpp_alias_templates);
#endif
#ifdef __cpp_attributes
    LEARN_SHOW_LANG(__cpp_attributes);
#endif
#ifdef __cpp_constexpr
    LEARN_SHOW_LANG(__cpp_constexpr);
#endif
#ifdef __cpp_decltype
    LEARN_SHOW_LANG(__cpp_decltype);
#endif
#ifdef __cpp_lambdas
    LEARN_SHOW_LANG(__cpp_lambdas);
#endif
#ifdef __cpp_rvalue_references
    LEARN_SHOW_LANG(__cpp_rvalue_references);
#endif
#ifdef __cpp_variadic_templates
    LEARN_SHOW_LANG(__cpp_variadic_templates);
#endif
#ifdef __cpp_static_assert
    LEARN_SHOW_LANG(__cpp_static_assert);
#endif
#ifdef __cpp_threadsafe_static_init
    LEARN_SHOW_LANG(__cpp_threadsafe_static_init);
#endif

    std::cout << "-- C++14 era --\n";
#ifdef __cpp_binary_literals
    LEARN_SHOW_LANG(__cpp_binary_literals);
#endif
#ifdef __cpp_digit_separators
    LEARN_SHOW_LANG(__cpp_digit_separators);
#endif
#ifdef __cpp_generic_lambdas
    LEARN_SHOW_LANG(__cpp_generic_lambdas);
#endif
#ifdef __cpp_init_captures
    LEARN_SHOW_LANG(__cpp_init_captures);
#endif
#ifdef __cpp_return_type_deduction
    LEARN_SHOW_LANG(__cpp_return_type_deduction);
#endif
#ifdef __cpp_variable_templates
    LEARN_SHOW_LANG(__cpp_variable_templates);
#endif
#ifdef __cpp_decltype_auto
    LEARN_SHOW_LANG(__cpp_decltype_auto);
#endif

    std::cout << "-- C++17 era --\n";
#ifdef __cpp_structured_bindings
    LEARN_SHOW_LANG(__cpp_structured_bindings);
#endif
#ifdef __cpp_if_constexpr
    LEARN_SHOW_LANG(__cpp_if_constexpr);
#endif
#ifdef __cpp_fold_expressions
    LEARN_SHOW_LANG(__cpp_fold_expressions);
#endif
#ifdef __cpp_inline_variables
    LEARN_SHOW_LANG(__cpp_inline_variables);
#endif
#ifdef __cpp_nontype_template_parameter_auto
    LEARN_SHOW_LANG(__cpp_nontype_template_parameter_auto);
#endif
#ifdef __cpp_capture_star_this
    LEARN_SHOW_LANG(__cpp_capture_star_this);
#endif
#ifdef __cpp_guaranteed_copy_elision
    LEARN_SHOW_LANG(__cpp_guaranteed_copy_elision);
#endif
#ifdef __cpp_aligned_new
    LEARN_SHOW_LANG(__cpp_aligned_new);
#endif

    std::cout << "-- C++20 era --\n";
#ifdef __cpp_concepts
    LEARN_SHOW_LANG(__cpp_concepts);
#endif
#ifdef __cpp_impl_coroutine
    LEARN_SHOW_LANG(__cpp_impl_coroutine);
#endif
#ifdef __cpp_modules
    LEARN_SHOW_LANG(__cpp_modules);
#endif
#ifdef __cpp_lib_concepts
    // library — listed only if leaked; primary dump is in library topic
#endif
#ifdef __cpp_consteval
    LEARN_SHOW_LANG(__cpp_consteval);
#endif
#ifdef __cpp_constinit
    LEARN_SHOW_LANG(__cpp_constinit);
#endif
#ifdef __cpp_char8_t
    LEARN_SHOW_LANG(__cpp_char8_t);
#endif
#ifdef __cpp_designated_initializers
    LEARN_SHOW_LANG(__cpp_designated_initializers);
#endif
#ifdef __cpp_impl_three_way_comparison
    LEARN_SHOW_LANG(__cpp_impl_three_way_comparison);
#endif
#ifdef __cpp_generic_lambdas
    // already shown; C++20 template lambdas bump may appear on related macros
#endif
#ifdef __cpp_nontype_template_args
    LEARN_SHOW_LANG(__cpp_nontype_template_args);
#endif

    std::cout << "-- C++23 signature language macros --\n";
#ifdef __cpp_explicit_this_parameter
    LEARN_SHOW_LANG(__cpp_explicit_this_parameter);
#else
    std::cout << "  __cpp_explicit_this_parameter = <undefined>\n";
#endif
#ifdef __cpp_if_consteval
    LEARN_SHOW_LANG(__cpp_if_consteval);
#else
    std::cout << "  __cpp_if_consteval = <undefined>\n";
#endif
#ifdef __cpp_multidimensional_subscript
    LEARN_SHOW_LANG(__cpp_multidimensional_subscript);
#else
    std::cout << "  __cpp_multidimensional_subscript = <undefined>\n";
#endif
#ifdef __cpp_auto_cast
    LEARN_SHOW_LANG(__cpp_auto_cast);
#else
    std::cout << "  __cpp_auto_cast = <undefined>\n";
#endif
#ifdef __cpp_size_t_suffix
    LEARN_SHOW_LANG(__cpp_size_t_suffix);
#else
    std::cout << "  __cpp_size_t_suffix = <undefined>\n";
#endif
#ifdef __cpp_static_call_operator
    LEARN_SHOW_LANG(__cpp_static_call_operator);
#else
    std::cout << "  __cpp_static_call_operator = <undefined>\n";
#endif
#ifdef __cpp_named_character_escapes
    LEARN_SHOW_LANG(__cpp_named_character_escapes);
#else
    std::cout << "  __cpp_named_character_escapes = <undefined>\n";
#endif

    // Conditional compilation pattern (methodology core of stage14)
    std::cout << "-- conditional use pattern --\n";
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    std::cout << "  if consteval AVAILABLE (>= " << __cpp_if_consteval << ")\n";
#else
    std::cout << "  if consteval FALLBACK (use is_constant_evaluated)\n";
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    std::cout << "  constexpr >= 202211L (C++23 relaxations) value=" << __cpp_constexpr << '\n';
#elif defined(__cpp_constexpr)
    std::cout << "  constexpr present but < 202211L value=" << __cpp_constexpr << '\n';
#endif

    std::cout << "cpp_language_feature_macros OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/cpp_language_feature_macros", run>;

}  // namespace
