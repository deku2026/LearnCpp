// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C9 prefetch)
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : prefetch_intro
// Topic id : part6/c/section05/prefetch_intro
//
// 要点: 预取是提示；标准 C++ 无 portable prefetch 内建——用访问模式/软件流水演示思想。
//       不用 __builtin_prefetch（支线铁律：标准 C++）。
// 参考: cache-friendly loops, software pipelining

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

// 「软件预取」思想：提前读下一个节点，降低链表指针追逐延迟
struct Node {
    int value;
    Node* next;
};

int sum_list_lookahead(Node* head) {
    int sum = 0;
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;  // 尽早加载下一指针
        sum += cur->value;
        cur = next;
    }
    return sum;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C9 prefetch intro (portable patterns) ===\n";

    // 连续数组：硬件预取器通常已足够
    std::vector<int> v(1024);
    std::iota(v.begin(), v.end(), 1);
    long long s = 0;
    for (int x : v) s += x;
    assert(s == 1024LL * 1025 / 2);

    // 链表：手动提前读 next
    Node n3{3, nullptr};
    Node n2{2, &n3};
    Node n1{1, &n2};
    assert(sum_list_lookahead(&n1) == 6);

    std::cout << "  prefer contiguous layout; lookahead next pointer on lists\n";
    std::cout << "  (no compiler builtins — standard C++ only)\n";
    std::cout << "prefetch_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/prefetch_intro", run>;

}  // namespace
