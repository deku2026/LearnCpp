// Topic     : Ranges 算法投影(projection)参数
// Doc       : 第2部分-阶段10 · 步骤 11.3
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges
//
// 要点: sort/find 等先 proj 再比较; 成员指针常用投影。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

struct Person {
    std::string name;
    int age;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [projection_argument] ===\n";

    std::vector<Person> people{
        {"Carol", 35},
        {"Alice", 30},
        {"Bob", 25},
    };

    // 按 age 排序: {} = less, 第三参投影
    std::ranges::sort(people, {}, &Person::age);
    assert(people[0].name == "Bob");
    assert(people[2].name == "Carol");

    // 按 name 找
    [[maybe_unused]] auto it = std::ranges::find(people, "Alice", &Person::name);
    assert(it != people.end() && it->age == 30);

    // 按 name 字典序
    std::ranges::sort(people, {}, &Person::name);
    assert(people[0].name == "Alice" && people[2].name == "Carol");

    // 投影 + 自定义比较: 按 age 降序
    std::ranges::sort(people, std::ranges::greater{}, &Person::age);
    assert(people.front().age == 35);

    // 自由函数投影
    auto ages = people | std::views::transform(&Person::age);
    [[maybe_unused]] int sum = 0;
    for (int a : ages) sum += a;
    assert(sum == 25 + 30 + 35);

    // ranges::min/max 投影
    auto youngest = std::ranges::min(people, {}, &Person::age);
    assert(youngest.name == "Bob");

    std::cout << "[projection] sort/find/min by field OK\n";
    std::cout << "projection_argument: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/projection_argument", run>;

}  // namespace
