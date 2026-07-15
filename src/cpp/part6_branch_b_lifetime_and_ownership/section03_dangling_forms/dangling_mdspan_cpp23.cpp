// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 mdspan)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_mdspan_cpp23
// Topic id : part6/b/section03/dangling_mdspan_cpp23
//
// 要点: mdspan(C++23) 同样是非拥有多维视图——统一悬垂模型。
// 参考: https://en.cppreference.com/w/cpp/container/mdspan

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <mdspan>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling mdspan (C++23, safe contrasts) ===\n";

#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    // 拥有型缓冲 + mdspan 借用
    std::vector<int> storage(6);
    std::mdspan<int, std::extents<std::size_t, 2, 3>> m{storage.data(), 2, 3};
    // 部分实现用 dynamic extents:
    // std::mdspan m{storage.data(), std::extents<std::size_t, 2, 3>{}};

    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 2] = 6;
    assert(storage[0] == 1);
    assert(storage[1] == 2);
    assert(storage[5] == 6);

    // ❌ 危险模式（注释）:
    // auto bad() {
    //   std::vector<int> local(6);
    //   return std::mdspan(local.data(), 2, 3); // local 销毁 → 悬垂
    // }

    // 扩容会使 data() 指针失效
    auto* old_data = storage.data();
    storage.reserve(storage.capacity() + 32);
    storage.push_back(0);  // 可能重分配
    if (storage.data() != old_data) {
        // 旧 mdspan 失效——重建
        m = std::mdspan<int, std::extents<std::size_t, 2, 3>>{storage.data(), 2, 3};
    }
    assert(m.extent(0) == 2);
    assert(m.extent(1) == 3);

    std::cout << "  mdspan = multi-dim borrow; owner must outlive view\n";
#else
    // 回退：用裸指针+extents 语义说明（无 mdspan 时仍可编译）
    std::vector<int> storage(6);
    int* p = storage.data();
    p[0] = 1;
    p[5] = 6;
    assert(storage.front() == 1 && storage[5] == 6);
    std::cout << "  __cpp_lib_mdspan not available; semantic note only\n";
#endif

    std::cout << "dangling_mdspan_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_mdspan_cpp23", run>;

}  // namespace
