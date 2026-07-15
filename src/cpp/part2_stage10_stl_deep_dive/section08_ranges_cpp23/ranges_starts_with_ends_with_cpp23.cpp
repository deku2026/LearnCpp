// Topic     : ranges::starts_with / ends_with (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.2
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/starts_with
//
// 要点: 范围前缀/后缀判断; 支持投影。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_starts_with_ends_with_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_starts_ends_with) && __cpp_lib_ranges_starts_ends_with >= 202106L
    assert(std::ranges::starts_with(v, std::vector{1, 2}));
    assert(std::ranges::ends_with(v, std::vector{4, 5}));
    assert(!std::ranges::starts_with(v, std::vector{2, 3}));
    assert(!std::ranges::ends_with(v, std::vector{1, 2}));

    // 空模式恒真
    assert(std::ranges::starts_with(v, std::vector<int>{}));
    assert(std::ranges::ends_with(v, std::vector<int>{}));

    std::string s = "filename.cpp";
    assert(std::ranges::ends_with(s, std::string{".cpp"}));
    assert(std::ranges::starts_with(s, std::string{"file"}));

    std::cout << "[starts_with/ends_with] library path OK\n";
#else
    std::cout << "library not yet available, fallback demo (mismatch/equal)\n";
    auto starts_with = [](const auto& r, const auto& prefix) {
        if (std::ranges::size(prefix) > std::ranges::size(r)) return false;
        return std::ranges::equal(
            prefix, std::ranges::subrange(std::ranges::begin(r), std::ranges::begin(r) + std::ranges::size(prefix)));
    };
    auto ends_with = [](const auto& r, const auto& suffix) {
        if (std::ranges::size(suffix) > std::ranges::size(r)) return false;
        auto off = std::ranges::size(r) - std::ranges::size(suffix);
        return std::ranges::equal(suffix, std::ranges::subrange(std::ranges::begin(r) + off, std::ranges::end(r)));
    };

    assert(starts_with(v, std::vector{1, 2}));
    assert(ends_with(v, std::vector{4, 5}));
    assert(!starts_with(v, std::vector{2, 3}));
    assert(!ends_with(v, std::vector{1, 2}));
    assert(starts_with(v, std::vector<int>{}));
    assert(ends_with(v, std::vector<int>{}));

    std::string s = "filename.cpp";
    // string 自带 starts_with/ends_with (C++20)
    assert(s.ends_with(".cpp"));
    assert(s.starts_with("file"));

    std::cout << "[starts_with/ends_with] fallback OK\n";
#endif

    std::cout << "ranges_starts_with_ends_with_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_starts_with_ends_with_cpp23", run>;

}  // namespace
