// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.4 比较器 / 哈希 / 投影
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : custom_comparator_hash_projection
// Topic id : part3/section02/custom_comparator_hash_projection
//
// Refs:
//   https://en.cppreference.com/w/cpp/named_req/Compare
//   https://en.cppreference.com/w/cpp/utility/hash
//   https://en.cppreference.com/w/cpp/algorithm/ranges/sort
//
// 要点: 严格弱序; 自定义 hash+eq; ranges 投影省比较器。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

struct Point {
    int x{};
    int y{};
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
};

struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        // 简易组合哈希(教学用; 生产可用更好混合)
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};

struct Person {
    std::string name;
    int age{};
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/custom_comparator_hash_projection ===\n";

    // 自定义比较器: 降序 map; 必须严格弱序
    std::map<int, std::string, std::greater<int>> desc{{1, "a"}, {3, "c"}, {2, "b"}};
    assert(desc.begin()->first == 3);

    std::vector<int> v{1, 5, 3, 4, 2};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    assert(v.front() == 5 && v.back() == 1);
    std::cout << "[intro] greater comparator ok\n";

    // 自定义哈希
    std::unordered_set<Point, PointHash> set;
    set.insert(Point{1, 2});
    set.insert(Point{1, 2});  // 重复
    set.insert(Point{3, 4});
    assert(set.size() == 2);
    assert(set.contains(Point{1, 2}));
    std::cout << "[advanced] PointHash unordered_set size=" << set.size() << '\n';

    // 投影
    std::vector<Person> people{{"Zed", 20}, {"Amy", 40}, {"Bob", 30}};
    std::ranges::sort(people, std::less<>{}, &Person::name);
    assert(people.front().name == "Amy");
    std::ranges::sort(people, {}, &Person::age);
    assert(people.front().age == 20);

    std::cout << "[expert] projection: ranges::sort(people, {}, &Person::age)\n";
    std::cout << "custom_comparator_hash_projection: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/custom_comparator_hash_projection", run>;

}  // namespace
