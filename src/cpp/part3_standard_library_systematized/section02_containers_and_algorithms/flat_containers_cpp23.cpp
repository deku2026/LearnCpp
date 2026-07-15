// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.2 flat_map / flat_set (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : flat_containers_cpp23
// Topic id : part3/section02/flat_containers_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/container/flat_map
//   https://en.cppreference.com/w/cpp/container/flat_set
//   Feature-test: __cpp_lib_flat_map / __cpp_lib_flat_set
//
// 要点: 有序 vector 实现; 查找 O(log n); 插删 O(n); 缓存友好、查多改少。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<flat_map>)
#include <flat_map>
#endif
#if __has_include(<flat_set>)
#include <flat_set>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/flat_containers_cpp23 ===\n";

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<std::string, int> fm;
    fm.insert({"b", 2});
    fm.insert({"a", 1});
    fm.insert({"c", 3});
    assert(fm.size() == 3);
    assert(fm["a"] == 1);
    // 底层有序: begin 键最小
    assert(fm.begin()->first == "a");
    assert(fm.contains("b"));
    std::cout << "[intro] flat_map size=" << fm.size() << " first=" << fm.begin()->first << '\n';
#else
    // 降级示意: 有序 vector + lower_bound 语义
    std::vector<std::pair<std::string, int>> fm{{"a", 1}, {"b", 2}, {"c", 3}};
    assert(fm.front().first == "a");
    std::cout << "[intro] flat_map unavailable; sorted vector stand-in\n";
#endif

#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_set<int> fs{3, 1, 2, 2};
    assert(fs.size() == 3);  // 去重
    assert(fs.contains(2));
    assert(*fs.begin() == 1);
    std::cout << "[advanced] flat_set unique ordered elements\n";
#else
    std::vector<int> fs{1, 2, 3};
    assert(fs.size() == 3);
    std::cout << "[advanced] flat_set unavailable; sorted vector stand-in\n";
#endif

    std::cout << "[expert] use flat_* when lookup-heavy + modest size; "
                 "avoid when insert/erase is hot (O(n) moves)\n";
    std::cout << "flat_containers_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/flat_containers_cpp23", run>;

}  // namespace
