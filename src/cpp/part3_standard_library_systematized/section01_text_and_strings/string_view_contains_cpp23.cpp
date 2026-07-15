// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.2 string_view::contains (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_contains_cpp23
// Topic id : part3/section01/string_view_contains_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string_view/contains
//   Feature-test: __cpp_lib_string_contains >= 202011L

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string_view>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/string_view_contains_cpp23 ===\n";

    constexpr std::string_view sv = "the quick brown fox";

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    assert(sv.contains("quick"));
    assert(sv.contains('q'));
    assert(sv.contains(std::string_view{"fox"}));
    assert(!sv.contains("dog"));
    static_assert(std::string_view{"abc"}.contains('b'));
    std::cout << "[intro] string_view::contains available\n";
#else
    auto contains = [](std::string_view hay, std::string_view needle) {
        return hay.find(needle) != std::string_view::npos;
    };
    assert(contains(sv, "quick"));
    assert(!contains(sv, "dog"));
    std::cout << "[intro] contains unavailable; used find fallback\n";
#endif

    std::cout << "string_view_contains_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_contains_cpp23", run>;

}  // namespace
