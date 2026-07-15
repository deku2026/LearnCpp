// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : removed_in_cpp23
// Topic id : part2/stage14/section08/removed_in_cpp23
//
// C++23 *removals* (ill-formed / deleted — cannot execute the old form):
//  - multi-character / non-encodable wide character literals (P2362)
//  - mixed encoding-prefix string literal concatenation (P2201)
//  - GC / reachability library (P2186): declare_reachable, pointer_safety, ...
//  - comma operator inside [] became multidimensional subscript (semantic removal)
// This topic documents each removal and shows the portable replacement that *does*
// compile under C++23.

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

// Multidimensional subscript replacement for the old arr[i, j] comma trick
struct Grid {
    std::vector<int> flat;
    std::size_t cols = 0;
    int& operator[](std::size_t r, std::size_t c) { return flat[r * cols + c]; }
    int operator[](std::size_t r, std::size_t c) const { return flat[r * cols + c]; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++23 removals — replacements that still compile ===\n";

    // 1) Wide multi-character literals removed (P2362)
    // OLD (ill-formed in C++23):  wchar_t x = L'ab';
    // Replacement: use numeric codes or single code unit / char32_t strings.
    {
        const wchar_t single = L'A';
        const char32_t u = U'A';
        std::cout << "[P2362 wide multi-char REMOVED] use single code units: L'A'=" << static_cast<int>(single)
                  << " U'A'=" << static_cast<std::uint32_t>(u) << '\n';
        assert(single == L'A');
    }

    // 2) Mixed encoding-prefix concatenation removed (P2201)
    // OLD (ill-formed):  auto s = u"q" U"p";  or L"" u8""
    // Replacement: keep a single encoding prefix, or convert explicitly.
    {
        const char32_t* utf32 = U"qp";  // one prefix for whole literal
        const char16_t* utf16 = u"qp";
        const auto* utf8 = u8"qp";  // char8_t* in C++20+
        std::string narrow = "q";
        narrow += "p";
        std::cout << "[P2201 mixed concat REMOVED] U\"qp\"[0]=" << static_cast<unsigned>(utf32[0])
                  << " narrow=" << narrow << '\n';
        (void)utf16;
        (void)utf8;
        assert(narrow == "qp");
    }

    // 3) GC support removed (P2186)
    // OLD: std::declare_reachable / undeclare_reachable / declare_no_pointers /
    //      get_pointer_safety / pointer_safety enum — deleted from the library.
    // Replacement: rely on normal ownership (unique_ptr/shared_ptr/RAII); no GC API.
    {
        auto owned = std::make_unique<int>(42);
        std::cout << "[P2186 GC API REMOVED] use RAII/ownership; *owned=" << *owned << " (no declare_reachable)\n";
        assert(*owned == 42);
    }

    // 4) Comma in subscript no longer means comma-operator (multidim [])
    // OLD: arr[i, j] evaluating comma then single index
    // NEW: arr[i, j] is a two-argument operator[] call when overloaded
    {
        Grid g;
        g.cols = 3;
        g.flat.assign(2 * 3, 0);
        g[1, 2] = 9;
        // If you truly need the comma operator for a single index, parenthesize:
        int i = 0;
        int j = 4;
        std::vector<int> arr{0, 1, 2, 3, 4, 5};
        const int old_style_index = arr[(void(i), j)];  // comma operator still works in ( )
        std::cout << "[subscript comma REMOVED as [i,j]] grid[1,2]=" << g[1, 2]
                  << " parenthesized comma index=" << old_style_index << '\n';
        assert((g[1, 2] == 9) && old_style_index == 4);
    }

    std::cout << "removed_in_cpp23 documentation demos OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/removed_in_cpp23", run>;

}  // namespace
