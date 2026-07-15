// Topic     : views::zip —— 并行打包多个范围 (C++23, 验收点)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.1
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_zip_cpp23
// Topic id  : part2/stage10/section08/views_zip_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/zip_view
//             P2321

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_zip_cpp23] 入门：并行打包成 tuple ===\n";
    {
        std::vector<std::string> names{"Alice", "Bob", "Carol"};
        std::vector<int> ages{30, 25, 35};

        std::vector<std::string> lines;
        for (auto [name, age] : std::views::zip(names, ages)) {
            lines.push_back(name + ":" + std::to_string(age));
        }
        assert(lines.size() == 3);
        assert(lines[0] == "Alice:30" && lines[1] == "Bob:25" && lines[2] == "Carol:35");
        std::cout << "zip names×ages OK\n";
    }

    std::cout << "=== 进阶：最短截断 + 三路 zip ===\n";
    {
        std::vector<int> a{1, 2, 3, 4};
        std::vector<int> b{10, 20};
        int n = 0;
        for (auto [x, y] : std::views::zip(a, b)) {
            assert(y == x * 10);
            ++n;
        }
        assert(n == 2);  // 取最短

        std::vector<std::string> names{"Alice", "Bob", "Carol"};
        std::vector<int> ages{30, 25, 35};
        std::vector<char> tags{'A', 'B', 'C'};
        int sum = 0;
        for (auto [name, age, tag] : std::views::zip(names, ages, tags)) {
            if (tag == 'B') {
                assert(name == "Bob" && age == 25);
            }
            sum += age;
        }
        assert(sum == 90);
        std::cout << "min-length + 3-way OK\n";
    }

    std::cout << "=== 专家：category 取最弱 + 与 enumerate 组合 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        auto z = std::views::zip(v, v);
        static_assert(std::ranges::random_access_range<decltype(z)>);
        assert(std::get<0>(z[1]) == 2);

        // 与 filter 混合：zip 再 filter 会降 category
        auto filtered = std::views::zip(v, v) | std::views::filter([](const auto& t) { return std::get<0>(t) > 1; });
        static_assert(!std::ranges::random_access_range<decltype(filtered)>);

        std::vector<std::string> names{"Alice", "Bob", "Carol"};
        std::vector<int> ages{30, 25, 35};
        int bob_i = -1;
        for (auto [i, pair] : std::views::enumerate(std::views::zip(names, ages))) {
            auto [name, age] = pair;
            if (name == "Bob") {
                bob_i = static_cast<int>(i);
                assert(age == 25);
            }
        }
        assert(bob_i == 1);

#if defined(__cpp_lib_ranges_zip)
        std::cout << "__cpp_lib_ranges_zip=" << __cpp_lib_ranges_zip << '\n';
#endif
        std::cout << "category + enumerate combo OK\n";
    }

    std::cout << "[views_zip_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_zip_cpp23", run>;

}  // namespace
