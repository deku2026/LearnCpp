// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.2 string_view 范围构造 (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_range_constructor_cpp23
// Topic id : part3/section01/string_view_range_constructor_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view
//   P1989R2 — range constructor for basic_string_view
//   Feature-test: __cpp_lib_ranges_as_const / 看实现; 通常随 C++23 string_view
//
// 要点: 从 contiguous range(如 array/vector<char>/span) 直接构造 string_view。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/string_view_range_constructor_cpp23 ===\n";

    // C++23: template <class R> explicit basic_string_view(R&& r)
    // 要求 contiguous + sized + 元素可转 CharT
    std::array<char, 5> arr{'h', 'e', 'l', 'l', 'o'};
#if defined(__cpp_lib_string_view) || 1
    // 多数 C++23 库支持 range ctor; 也可用 iterator pair / data+size 兼容写法
    std::string_view from_arr{arr.begin(), arr.end()};
    assert(from_arr == "hello");

    // 明确 contiguous: data()+size
    std::vector<char> v{'w', 'o', 'r', 'l', 'd'};
    std::string_view from_vec{v.data(), v.size()};
    assert(from_vec == "world");

    std::span<const char> sp{arr};
    std::string_view from_span{sp.data(), sp.size()};
    assert(from_span == "hello");

    // 与 string 互操作
    std::string s{from_arr};
    assert(s == "hello");
    std::string_view again = s;
    assert(again == from_arr);
#endif

    std::cout << "[intro] range/iterator-pair construction of string_view ok\n";
    std::cout << "[expert] storage must stay alive for the view's lifetime\n";
    std::cout << "string_view_range_constructor_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_range_constructor_cpp23", run>;

}  // namespace
