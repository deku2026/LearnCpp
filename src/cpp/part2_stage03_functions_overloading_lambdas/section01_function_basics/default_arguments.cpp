// Topic     : 默认实参
// Doc       : 第2部分-阶段3 · 步骤 3.1
// cppreference: https://en.cppreference.com/cpp/language/default_arguments
//
// 要点: 从右往左连续给默认值；不可跳过中间参数；通常只在声明处写一次；
//       默认实参在调用点求值，不参与函数签名/重载集身份。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 声明处给默认值（教学里声明=定义写在一起亦可；多文件工程声明/定义要分开且默认只写声明处）
void greet(const std::string& name, const std::string& greeting = "Hello") {
    // 体中可用 greeting 的默认或调用者传入值
    (void)name;
    (void)greeting;
}

[[maybe_unused]] int scale(int value, int factor = 2, int offset = 0) {
    return value * factor + offset;
}

// 默认实参在【调用点】求值：每次省略时重新执行表达式
int next_id() {
    static int n = 0;
    return ++n;
}

void tag(int id = next_id()) {
    (void)id;
}

// ⚠️ 默认实参 + 重载易歧义（演示概念；真正歧义调用不要放进可编译路径）
void g_one(int x) {
    (void)x;
}
// void g_one(int x, int y = 0);  // 若同时存在，g_one(5) 歧义

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [default_arguments] ===\n";

    // -------------------------------------------------------------------------
    // §入门：省略尾部参数
    // -------------------------------------------------------------------------
    greet("Alice");           // greeting 用 "Hello"
    greet("Bob", "Hi");       // 显式提供
    assert(scale(10) == 20);  // factor=2, offset=0
    assert(scale(10, 3) == 30);
    assert(scale(10, 3, 1) == 31);
    std::cout << "[intro] trailing defaults applied left-to-right fill of omitted args\n";

    // -------------------------------------------------------------------------
    // §进阶：规则与求值时机
    // -------------------------------------------------------------------------
    // 规则：一旦某参数有默认，其右侧都必须有默认。不能写 void f(int a = 1, int b);
    // 不能跳过中间：无 f(, 5) 语法；若要「只改 offset」需重载或命名参数惯用法（结构体）。
    assert(scale(5, 1) == 5);

    // 每次省略默认实参都会在调用点求值
    const int before = next_id();  // 先推进静态计数，便于断言「tag 省略时再调 next_id」
    (void)before;
    tag();  // 调用 next_id
    tag();  // 再调用 next_id
    // next_id 已被 greet 路径外再调用：至少又增加了 2
    const int after = next_id();
    assert(after >= 3);
    std::cout << "[advanced] default args evaluated at each call site; after id=" << after << '\n';

    // -------------------------------------------------------------------------
    // §专家：不参与签名；与重载的边界
    // -------------------------------------------------------------------------
    // 1) 默认实参不是函数类型的一部分：void(*)(int) 与带默认的 void f(int=0) 在类型上同为 void(int)。
    // 2) 多 TU：默认实参可在后续声明中「追加」右侧默认，但同一参数不能重复指定不同默认（ODR）。
    // 3) 与重载：void h(int); void h(int, int=0); 对 h(1) 产生歧义——避免「带默认的重载」叠无默认版。
    using Fn = void (*)(const std::string&, const std::string&);
    Fn fp = greet;  // 函数指针类型不含默认实参
    fp("Zed", "Yo");
    g_one(42);

    std::cout << "[expert] defaults not part of signature; watch overload ambiguity\n";
    std::cout << "=== default_arguments: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/default_arguments", run>;

}  // namespace
