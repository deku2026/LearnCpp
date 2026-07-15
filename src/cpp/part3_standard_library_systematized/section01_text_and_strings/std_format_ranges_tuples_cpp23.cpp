// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.4 format ranges/tuples (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part3/section01/std_format_ranges_tuples_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/format/ranges_formatter
//   P2286R8 — Formatting Ranges
//   Feature-test: __cpp_lib_format_ranges >= 202207L

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<format>)
#include <format>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_format_ranges_tuples_cpp23 ===\n";

#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    const std::vector<int> v{1, 2, 3};
    const std::string vs = std::format("{}", v);
    // 典型: "[1, 2, 3]"
    assert(vs.find('1') != std::string::npos);
    assert(vs.find('3') != std::string::npos);
    std::cout << "[intro] vector -> " << vs << '\n';

    const auto t = std::tuple{1, 2.5, "hi"};
    const std::string ts = std::format("{}", t);
    assert(ts.find('1') != std::string::npos);
    std::cout << "[advanced] tuple -> " << ts << '\n';

    const auto p = std::pair{std::string{"k"}, 42};
    const std::string ps = std::format("{}", p);
    assert(ps.find('k') != std::string::npos);
    std::cout << "[advanced] pair -> " << ps << '\n';

    const std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    const std::string ms = std::format("{}", m);
    std::cout << "[expert] map -> " << ms << '\n';
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // 无 ranges formatter 时手写
    const std::vector<int> v{1, 2, 3};
    std::string vs = "[";
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) {
            vs += ", ";
        }
        vs += std::format("{}", v[i]);
    }
    vs += "]";
    assert(vs == "[1, 2, 3]");
    std::cout << "[intro] format_ranges unavailable; manual " << vs << '\n';
#else
    std::cout << "[intro] format not available\n";
#endif

    std::cout << "std_format_ranges_tuples_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_ranges_tuples_cpp23", run>;

}  // namespace
