// Topic     : nullptr 替代 NULL / 0
// Doc       : 第2部分-阶段2 · 步骤 1.5
// cppreference: https://en.cppreference.com/cpp/language/nullptr
//               https://en.cppreference.com/cpp/types/nullptr_t
//
// 要点: nullptr 类型是 std::nullptr_t；可隐式转为任意指针类型，不能当整数；
//       重载决议中 0/NULL 常误选 int 重载，nullptr 选指针重载。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace {

int overload_probe(int) {
    return 1;
}
int overload_probe(char*) {
    return 2;
}
int overload_probe(std::nullptr_t) {
    return 3;
}

void take_ptr(const int*) {}
// void take_int_only(int) {}  // 对比：nullptr 不能传给纯 int 形参

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [nullptr_replaces_null_zero] ===\n";

    // -------------------------------------------------------------------------
    // 入门：统一空指针字面值
    // -------------------------------------------------------------------------
    int* p = nullptr;
    char* q = nullptr;
    void* raw = nullptr;
    assert(p == nullptr && q == nullptr && raw == nullptr);
    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);

    // 可与指针比较；布尔语境为 false
    if (!p) {
        std::cout << "[intro] null pointer is falsy\n";
    }

    // -------------------------------------------------------------------------
    // 进阶：重载决议 — 0 / NULL 的陷阱
    // -------------------------------------------------------------------------
    // 历史：NULL 常是 #define NULL 0 或 0L；字面值 0 既是 int 又是空指针常量
    assert(overload_probe(0) == 1);  // 优先匹配 int（精确匹配）
    // overload_probe(NULL) 在 NULL==0 时同样走 int——这是经典坑

    assert(overload_probe(nullptr) == 3);  // 精确匹配 nullptr_t 更优
    // 若没有 nullptr_t 重载，则走 char*（指针），不会走 int：
    // 删除 overload_probe(std::nullptr_t) 后，overload_probe(nullptr) → 2

    int only_int = overload_probe(static_cast<int>(0));
    int only_ptr = overload_probe(static_cast<char*>(nullptr));
    assert(only_int == 1 && only_ptr == 2);

    take_ptr(nullptr);  // OK
    // take_ptr(0);     // 许多编译器警告/在严格模式下不推荐；语义上依赖空指针常量规则
    std::cout << "[advanced] nullptr picks pointer/nullptr_t overloads; 0 picks int\n";

    // -------------------------------------------------------------------------
    // 专家：nullptr_t 的性质与模板
    // -------------------------------------------------------------------------
    std::nullptr_t nt = nullptr;
    void* from_nt = nt;  // nullptr_t → 任意指针
    assert(from_nt == nullptr);
    // int n = nullptr;          // ❌ 不能转整数
    // std::size_t s = nullptr;  // ❌

    // 模板里用 is_null_pointer / is_same
    static_assert(std::is_null_pointer_v<std::nullptr_t>);
    static_assert(std::is_same_v<std::remove_cv_t<decltype(nt)>, std::nullptr_t>);

    // 函数指针 / 成员指针也可置空
    int (*fp)(int) = nullptr;
    assert(fp == nullptr);
    (void)fp;

    // 现代风格：接口里“可空指针”用 nullptr 初始化；更安全则 optional / 智能指针
    std::cout << "[expert] never use 0/NULL as null pointer in new code\n";
    std::cout << "=== nullptr_replaces_null_zero: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/nullptr_replaces_null_zero", run>;

}  // namespace
