// LearnCpp topic
// Doc      : 第6部分-支线G · G9.1 pointer provenance
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : pointer_provenance
// Topic id : part6/g/section04/pointer_provenance
//
// 要点: 指针携带"来源/归属"信息, 不止地址位;
//       从无关对象指针"凑出"地址可能 UB。
// 参考: PNVI-ae-udi; PVI models; WG14/WG21 provenance papers

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G9 pointer provenance ===\n";

    int a = 1;
    int b = 2;
    int* pa = &a;
    [[maybe_unused]] int* pb = &b;
    assert(*pa == 1 && *pb == 2);

    // 合法: 同一数组内指针算术
    int arr[3]{10, 20, 30};
    int* p0 = &arr[0];
    [[maybe_unused]] int* p2 = p0 + 2;
    assert(*p2 == 30);
    // 允许 one-past-end 指针, 不可解引用
    [[maybe_unused]] int* end = arr + 3;
    assert(end - p0 == 3);

    // 危险直觉 (不演示):
    // uintptr_t ua = (uintptr_t)&a;
    // int* forged = (int*)(ua + offset_to_b); *forged;
    // 即使地址碰巧等于 &b, 来源不对仍可能 UB。

    // 合法: 整数往返同一指针 (实现定义细节多, 但常见平台 OK)
    std::uintptr_t u = reinterpret_cast<std::uintptr_t>(pa);
    [[maybe_unused]] int* pa2 = reinterpret_cast<int*>(u);
    assert(pa2 == pa);
    assert(*pa2 == 1);

    std::cout << "  optimizers use provenance to prove non-aliasing\n";
    std::cout << "  do not manufacture pointers from unrelated addresses\n";
    std::cout << "pointer_provenance: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/pointer_provenance", run>;

}  // namespace
