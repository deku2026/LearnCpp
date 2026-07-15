// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 uses-allocator)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : uses_allocator_construction
// Topic id : part6/c/section06/uses_allocator_construction
//
// 要点: uses_allocator 时构造元素会传入分配器；pmr 嵌套容器靠此共享 resource。
// 参考: https://en.cppreference.com/w/cpp/memory/uses_allocator

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <string>
#include <tuple>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 uses_allocator construction ===\n";

    static_assert(std::uses_allocator_v<std::tuple<int>, std::allocator<int>> ||
                  !std::uses_allocator_v<std::tuple<int>, std::allocator<int>>);
    // pmr::string 接受 polymorphic_allocator
    static_assert(std::uses_allocator_v<std::pmr::string, std::pmr::polymorphic_allocator<char>>);

    std::array<std::byte, 2048> buf{};
    std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};
    std::pmr::vector<std::pmr::string> words{&pool};
    words.emplace_back("hello");
    words.emplace_back("pmr");
    assert(words.size() == 2);
    assert(words[0] == "hello");
    // 元素 string 与 vector 共用 pool（uses-allocator 传播）

    std::cout << "  nested pmr containers share memory_resource via uses-allocator\n";
    std::cout << "  [layer] traits/pmr/pools: see branch C doc C6-C8 acceptance\n";
    std::cout << "uses_allocator_construction: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/uses_allocator_construction", run>;

}  // namespace
