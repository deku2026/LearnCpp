// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section05_features_by_standard_cpp23_language
// Item     : cpp23_language_feature_checklist
// Topic id : part2/stage14/section05/cpp23_language_feature_checklist
//
// C++23 language signature checklist with real micro-demos.
// Guard missing pieces via __cpp_* (deducing this, if consteval, multidimensional
// subscript, auto(x), size_t suffixes, static operator(), assume, ...).

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <version>

namespace {

// ----- deducing this -----
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
struct Deducing {
    int value = 0;
    auto&& get(this auto&& self) { return self.value; }
    int add(this const Deducing& self, int x) { return self.value + x; }
};
#endif

// ----- multidimensional operator[] -----
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
struct Matrix2 {
    int data[2][3]{};
    int& operator[](std::size_t i, std::size_t j) { return data[i][j]; }
    const int& operator[](std::size_t i, std::size_t j) const { return data[i][j]; }
};
#endif

// ----- static operator() / operator[] -----
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
struct StaticCall {
    static int operator()(int x) { return x + 1; }
};
#endif

// ----- if consteval -----
constexpr int path_demo(int x) {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return x * 100;
    } else {
        return x + 1;
    }
#else
    if (std::is_constant_evaluated()) {
        return x * 100;
    }
    return x + 1;
#endif
}

// ----- label at end of compound statement -----
int end_label_demo(int x) {
    if (x < 0) {
        goto end;
    }
    x += 1;
end:
    return x;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++23 language feature checklist ===\n";

    // 1) deducing this
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    {
        Deducing d{41};
        d.get() = 41;
        const Deducing cd{2};
        std::cout << "[deducing this] add=" << d.add(1) << " const.get=" << cd.get()
                  << " macro=" << __cpp_explicit_this_parameter << '\n';
        assert(d.add(1) == 42);
    }
#else
    std::cout << "[deducing this] SKIP (no __cpp_explicit_this_parameter)\n";
#endif

    // 2) if consteval
    {
        constexpr int c = path_demo(2);
        // Prevent the compiler from constant-folding the "runtime" call
        // (constexpr fn + constant args can still be evaluated at compile time).
        volatile int runtime_x = 2;
        const int r = path_demo(runtime_x);
        std::cout << "[if consteval] c=" << c << " r=" << r << '\n';
        assert(c == 200 && r == 3);
    }

    // 3) auto(x) decay-copy
#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    {
        const int arr[] = {1, 2, 3};
        auto decayed = auto(arr);  // decays to pointer
        static_assert(std::is_same_v<decltype(decayed), const int*>);
        std::string s = "hi";
        auto copy = auto(s);
        s.clear();
        std::cout << "[auto(x)] copy=" << copy << " s.empty=" << s.empty() << " decayed[0]=" << decayed[0] << '\n';
        assert(copy == "hi");
    }
#else
    std::cout << "[auto(x)] SKIP\n";
#endif

    // 4) multidimensional subscript
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
    {
        Matrix2 m{};
        m[1, 2] = 7;
        std::cout << "[multidim []] m[1,2]=" << m[1, 2] << " macro=" << __cpp_multidimensional_subscript << '\n';
        assert((m[1, 2] == 7));
    }
#else
    std::cout << "[multidim []] SKIP\n";
#endif

    // 5) static operator()
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    {
        std::cout << "[static operator()] " << StaticCall{}(41) << " macro=" << __cpp_static_call_operator << '\n';
        assert(StaticCall{}(41) == 42);
    }
#else
    std::cout << "[static operator()] SKIP\n";
#endif

    // 6) [[assume]]
#if __has_cpp_attribute(assume) >= 202207L
    {
        int x = 5;
        [[assume(x > 0)]];
        std::cout << "[assume] x=" << x << " attr=" << __has_cpp_attribute(assume) << '\n';
    }
#else
    std::cout << "[assume] SKIP (__has_cpp_attribute(assume)=" << __has_cpp_attribute(assume) << ")\n";
#endif

    // 7) size_t / ptrdiff_t literal suffixes uz / z
#if defined(__cpp_size_t_suffix) && __cpp_size_t_suffix >= 202011L
    {
        auto n = 42uz;
        auto d = -3z;
        static_assert(std::is_same_v<decltype(n), std::size_t>);
        static_assert(std::is_same_v<decltype(d), std::ptrdiff_t>);
        std::cout << "[uz/z suffixes] n=" << n << " d=" << d << " macro=" << __cpp_size_t_suffix << '\n';
    }
#else
    std::cout << "[uz/z suffixes] SKIP\n";
#endif

    // 8) named universal character escapes \N{} (if supported)
#if defined(__cpp_named_character_escapes) && __cpp_named_character_escapes >= 202207L
    {
        const char* greek = "\N{GREEK CAPITAL LETTER ALPHA}";
        std::cout << "[named escapes] alpha_bytes_len=" << std::char_traits<char>::length(greek)
                  << " macro=" << __cpp_named_character_escapes << '\n';
    }
#else
    std::cout << "[named escapes] SKIP\n";
#endif

    // 9) range-for temporary lifetime extension (C++23)
    // Temporary from factory lives for whole loop.
    auto make_vec = [] { return std::vector<int>{1, 2, 3}; };
    int sum = 0;
    for (int x : make_vec()) {
        sum += x;
    }
    std::cout << "[range-for temp lifetime] sum=" << sum << '\n';
    assert(sum == 6);

    // 10) init-statement alias declaration (C++23) on range-for
    {
        int alias_sum = 0;
        for (using V = std::vector<int>; int x : V{10, 20, 12}) {
            alias_sum += x;
        }
        std::cout << "[for init alias-decl] sum=" << alias_sum << '\n';
        assert(alias_sum == 42);
    }

    // 11) #elifdef / #elifndef / #warning (preprocessor — report compile-time presence)
#ifdef __cpp_if_consteval
#elif defined(__cpp_explicit_this_parameter)
#endif
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    std::cout << "[constexpr 23] __cpp_constexpr=" << __cpp_constexpr << " (static locals / non-literal relaxations)\n";
#endif

    // 12) statement end label
    std::cout << "[end label] " << end_label_demo(41) << " / " << end_label_demo(-5) << '\n';
    assert(end_label_demo(41) == 42);

    // 13) lambda static call operator (C++23) if available
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    auto static_lam = [](int x) static { return x * 2; };
    std::cout << "[static lambda] " << static_lam(21) << '\n';
    assert(static_lam(21) == 42);
#endif

    std::cout << "C++23 language checklist OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section05/cpp23_language_feature_checklist", run>;

}  // namespace
