// LearnCpp topic
// Doc      : 第6部分-支线G · G1.3 extern "C"
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : extern_c_link
// Topic id : part6/g/section01/extern_c_link
//
// 要点: extern "C" 关闭 mangling、使用 C 语言链接; FFI / dlsym 友好。
// 参考: [dcl.link]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

extern "C" {
int c_add(int a, int b) {
    return a + b;
}
const char* c_version(void) {
    return "1.0";
}
}

// 不可重载: C 链接无函数重载
// extern "C" int c_add(double, double); // 冲突

// 函数指针类型也带语言链接
extern "C" typedef int (*CAddFn)(int, int);

namespace api {
// 命名空间内仍可 extern "C"
extern "C" int api_open(const char*) {
    return 1;
}
}  // namespace api

// 从 C++ 调用
int cxx_use_c() {
    CAddFn fn = &c_add;
    return fn(2, 3);
}

int run_impl() {
    std::cout << "=== G1 extern \"C\" linkage ===\n";

    assert(c_add(2, 40) == 42);
    assert(std::string(c_version()) == "1.0");
    assert(cxx_use_c() == 5);
    assert(api::api_open("x") == 1);

    // C++ 重载仍在 C++ 链接下
    struct S {
        static int f(int x) { return x; }
        static int f(double x) { return static_cast<int>(x) + 100; }
    };
    assert(S::f(1) == 1);
    assert(S::f(1.5) == 101);

    std::cout << "  use for: C ABI exports, plugins (dlsym), stable shared API\n";
    std::cout << "  cannot overload; no templates with C linkage\n";
    std::cout << "extern_c_link: OK\n";
    return 0;
}

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return run_impl();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/extern_c_link", run>;

}  // namespace
