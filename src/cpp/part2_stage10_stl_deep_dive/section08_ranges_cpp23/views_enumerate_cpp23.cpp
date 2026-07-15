// Topic     : views::enumerate —— 带下标遍历 (C++23, 验收点)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.1
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_enumerate_cpp23
// Topic id  : part2/stage10/section08/views_enumerate_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/enumerate_view
//             P2164

#include "learn/topic_registry.hpp"

#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <version>

#if !defined(__cpp_lib_ranges_zip) || !(__cpp_lib_ranges_zip)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::views::zip not available (__cpp_lib_ranges_zip)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_enumerate_cpp23", run>;
}  // namespace
#else
namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_enumerate_cpp23] 入门：(index, value) ===\n";
    {
        std::vector<std::string> names{"Alice", "Bob", "Carol"};
        std::vector<std::string> lines;
        for (auto [i, name] : std::views::enumerate(names)) {
            lines.push_back("[" + std::to_string(i) + "] " + name);
        }
        assert(lines[0] == "[0] Alice");
        assert(lines[1] == "[1] Bob");
        assert(lines[2] == "[2] Carol");
        std::cout << "index+value OK\n";
    }

    std::cout << "=== 进阶：管道形式 + zip 组合（验收）===\n";
    {
        std::vector<std::string> names{"Alice", "Bob", "Carol"};
        std::vector<int> ages{30, 25, 35};

        [[maybe_unused]] int bob_i = -1;
        for (auto [i, pair] : std::views::enumerate(std::views::zip(names, ages))) {
            auto [name, age] = pair;
            if (name == "Bob") {
                bob_i = static_cast<int>(i);
                assert(age == 25);
            }
        }
        assert(bob_i == 1);

        [[maybe_unused]] int last = -1;
        for (auto [i, x] : std::views::iota(10, 13) | std::views::enumerate) {
            assert(x == 10 + static_cast<int>(i));
            last = static_cast<int>(i);
        }
        assert(last == 2);
        std::cout << "zip+enumerate + pipe form OK\n";
    }

    std::cout << "=== 专家：下标类型 + 写回底层 ===\n";
    {
        std::vector<int> v{10, 20, 30};
        for (auto [i, x] : std::views::enumerate(v)) {
            // x 通常是引用代理，可写回
            x = static_cast<int>(i);
        }
        assert((v == std::vector<int>{0, 1, 2}));

        // 过滤后 enumerate：下标是过滤后序列的下标，不是原容器下标
        std::vector<int> src{1, 2, 3, 4, 5};
        std::vector<std::pair<std::ptrdiff_t, int>> pairs;
        for (auto [i, x] : src | std::views::filter([](int x) { return x % 2 == 0; }) | std::views::enumerate) {
            pairs.emplace_back(i, x);
        }
        assert(pairs.size() == 2);
        assert(pairs[0].first == 0 && pairs[0].second == 2);
        assert(pairs[1].first == 1 && pairs[1].second == 4);

#if defined(__cpp_lib_ranges_enumerate)
        std::cout << "__cpp_lib_ranges_enumerate=" << __cpp_lib_ranges_enumerate << '\n';
#endif
        std::cout << "write-back + post-filter indices OK\n";
    }

    std::cout << "[views_enumerate_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_enumerate_cpp23", run>;

}  // namespace
#endif  // __cpp_lib_ranges_zip
