// Topic     : ranges::contains / contains_subrange (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.2
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/contains
//
// 要点: 元素/子序列包含; 比 find!=end 更直白。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_contains_and_subrange_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_contains) && __cpp_lib_ranges_contains >= 202207L
    assert(std::ranges::contains(v, 3));
    assert(!std::ranges::contains(v, 9));

    assert(std::ranges::contains_subrange(v, std::vector{2, 3, 4}));
    assert(!std::ranges::contains_subrange(v, std::vector{2, 4}));  // 非连续子序列

    std::string s = "abracadabra";
    assert(std::ranges::contains(s, 'c'));
    assert(std::ranges::contains_subrange(s, std::string{"cada"}));

    // 投影：contains(r, value, proj) —— 无 pred 参数
    struct P {
        int id;
    };
    std::vector<P> ps{{1}, {2}, {3}};
    assert(std::ranges::contains(ps, 2, &P::id));

    std::cout << "[contains/contains_subrange] library path OK\n";
#else
    // MSVC STL 部分版本尚未提供 ranges::contains*：用 find / search 等价演示
    std::cout << "library not yet available, fallback demo (find/search)\n";
    assert(std::ranges::find(v, 3) != v.end());
    assert(std::ranges::find(v, 9) == v.end());

    std::vector needle{2, 3, 4};
    assert(std::ranges::search(v, needle).begin() != v.end());
    std::vector non_contig{2, 4};
    assert(std::ranges::search(v, non_contig).begin() == v.end());

    std::string s = "abracadabra";
    assert(std::ranges::find(s, 'c') != s.end());
    std::string sub{"cada"};
    assert(std::ranges::search(s, sub).begin() != s.end());

    struct P {
        int id;
    };
    std::vector<P> ps{{1}, {2}, {3}};
    assert(std::ranges::find(ps, 2, {}, &P::id) != ps.end());

    std::cout << "[contains/contains_subrange] fallback OK\n";
#endif

    std::cout << "ranges_contains_and_subrange_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_contains_and_subrange_cpp23", run>;

}  // namespace
