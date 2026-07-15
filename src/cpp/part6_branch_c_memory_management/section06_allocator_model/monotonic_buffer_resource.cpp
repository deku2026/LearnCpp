// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C8 monotonic_buffer_resource)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : monotonic_buffer_resource
// Topic id : part6/c/section06/monotonic_buffer_resource
//
// 要点: 只增长、销毁时整块释放；可叠栈 buffer，热路径极快。
// 参考: https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <memory_resource>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C8 monotonic_buffer_resource ===\n";

    std::array<std::byte, 2048> buffer{};
    std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size()};

    std::pmr::vector<int> v{&pool};
    for (int i = 0; i < 100; ++i) v.push_back(i);
    assert(v.size() == 100);
    assert(v[42] == 42);

    std::pmr::vector<std::pmr::string> words{&pool};
    words.emplace_back("frame");
    words.emplace_back("temp");
    assert(words[0] == "frame");

    // release 丢弃所有分配（容器应已销毁或不再使用）
    // 此处 words/v 仍在用，等作用域结束再由 pool 析构释放

    std::cout << "  bump allocation; no per-block free until resource dies\n";
    std::cout << "monotonic_buffer_resource: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/monotonic_buffer_resource", run>;

}  // namespace
