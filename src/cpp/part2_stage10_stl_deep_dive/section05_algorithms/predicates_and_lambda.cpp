// Topic     : 谓词与 lambda：定制算法比较 / 过滤 / 计数
// Doc       : 第2部分-阶段10 · 步骤 5.2
// cppreference: https://en.cppreference.com/cpp/algorithm
//               https://en.cppreference.com/w/cpp/language/lambda
//
// 要点: 现代 C++ 几乎都用 lambda 作谓词/比较器; 捕获要小心生命周期。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Person {
    std::string name;
    int age;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [predicates_and_lambda] ===\n";

    std::vector<int> v{5, 2, 8, 1, 9};

    // 自定义比较: 降序
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    assert((v == std::vector<int>{9, 8, 5, 2, 1}));

    // find_if / count_if
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });
    assert(it != v.end() && *it == 9);
    const int evens = static_cast<int>(std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }));
    assert(evens == 2);

    // 状态捕获: 闭包可带状态(注意别产生数据竞争)
    int threshold = 5;
    auto big = std::count_if(v.begin(), v.end(), [threshold](int x) { return x > threshold; });
    assert(big == 2);

    // 函数对象 / std::function 也可作谓词
    std::greater<> gt;
    assert(std::is_sorted(v.begin(), v.end(), gt));

    // 结构体字段比较
    std::vector<Person> people{
        {"Carol", 35},
        {"Alice", 30},
        {"Bob", 25},
    };
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) { return a.age < b.age; });
    assert(people.front().name == "Bob");
    assert(people.back().name == "Carol");

    auto bob = std::find_if(people.begin(), people.end(), [](const Person& p) { return p.name == "Bob"; });
    assert(bob != people.end() && bob->age == 25);

    // remove_if 谓词
    std::vector<int> nums{1, 2, 3, 4, 5, 6};
    nums.erase(std::remove_if(nums.begin(), nums.end(), [](int x) { return x % 2 != 0; }), nums.end());
    assert((nums == std::vector<int>{2, 4, 6}));

    std::cout << "[lambda] sort/find_if/count_if/remove_if with predicates OK\n";
    std::cout << "predicates_and_lambda: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/predicates_and_lambda", run>;

}  // namespace
