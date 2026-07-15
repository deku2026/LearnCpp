// Topic     : string_view 迭代器对 / 范围构造 (C++23, P1989)
// Doc       : 第2部分-阶段10 · 步骤 6.3
// cppreference: https://en.cppreference.com/cpp/string/basic_string_view/basic_string_view
//
// 要点: string_view(it, sent) 从连续字符迭代器构造。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_iterator_range_constructor_cpp23] ===\n";

    std::vector<char> chars{'H', 'e', 'l', 'l', 'o'};
    std::string_view sv(chars.begin(), chars.end());  // C++23
    assert(sv == "Hello");
    assert(sv.data() == chars.data());

    // 子范围
    std::string_view mid(chars.begin() + 1, chars.begin() + 4);  // "ell"
    assert(mid == "ell");

    // 从 string 迭代器
    std::string s = "C++23";
    std::string_view from_str(s.begin(), s.end());
    assert(from_str == s);

    // 数组
    char buf[] = {'a', 'b', 'c', 'd'};
    std::string_view from_arr(std::begin(buf), std::end(buf));
    assert(from_arr == "abcd");

    std::cout << "[P1989] vector<char>/string/array iterator-pair OK: " << sv << '\n';
    std::cout << "string_view_iterator_range_constructor_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06/string_view_iterator_range_constructor_cpp23", run>;

}  // namespace
