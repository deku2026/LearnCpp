// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库2 · 2.4 异质 erase / extract (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : heterogeneous_erase_extract_cpp23
// Topic id : part3/section02/heterogeneous_erase_extract_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/container/map/erase
//   https://en.cppreference.com/w/cpp/container/node_handle
//   P2077 — Heterogeneous erasure overloads for associative containers
//
// 要点: 透明比较器下用 string_view 查找/删除 string 键, 不构造临时 string;
//       extract 取出 node_handle 可改键再插回。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section02/heterogeneous_erase_extract_cpp23 ===\n";

    // 透明比较器 is_transparent 启用异质查找
    std::map<std::string, int, std::less<>> m{
        {"alpha", 1},
        {"beta", 2},
        {"gamma", 3},
    };

    // 异质 find: 不构造临时 std::string
    auto it = m.find(std::string_view{"beta"});
    assert(it != m.end() && it->second == 2);
    std::cout << "[intro] heterogeneous find(\"beta\")=" << it->second << '\n';

    // 异质 erase (C++23 在支持透明比较时提供)
    const auto erased = m.erase(std::string_view{"alpha"});
    assert(erased == 1);
    assert(!m.contains(std::string_view{"alpha"}));
    std::cout << "[advanced] heterogeneous erase alpha, size=" << m.size() << '\n';

    // extract: 取出节点, 修改 key, 再 insert
    auto nh = m.extract("beta");
    assert(!nh.empty());
    nh.key() = "BETA";
    const auto ins = m.insert(std::move(nh));
    assert(ins.inserted);
    assert(m.contains("BETA"));
    assert(!m.contains("beta"));

    std::cout << "[expert] extract/node_handle renames key without "
                 "allocate+delete pair\n";
    std::cout << "heterogeneous_erase_extract_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/heterogeneous_erase_extract_cpp23", run>;

}  // namespace
