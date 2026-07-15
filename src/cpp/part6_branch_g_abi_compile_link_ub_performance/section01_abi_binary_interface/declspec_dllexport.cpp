// LearnCpp topic
// Doc      : 第6部分-支线G · G2 MSVC dllexport
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : declspec_dllexport
// Topic id : part6/g/section01/declspec_dllexport
//
// 要点: Windows DLL 用 __declspec(dllexport/dllimport);
//       构建 DLL 时 export, 使用方 import。
// 参考: Microsoft docs dllexport/dllimport

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

#if defined(_WIN32)
#if defined(LEARN_BUILDING_FAKE_DLL)
#define DEMO_API __declspec(dllexport)
#else
// 本教学单元编进同一可执行文件, 用 dllexport 仅作语法演示
#define DEMO_API __declspec(dllexport)
#endif
#else
#define DEMO_API
#endif

// 跨平台宏模式 (与真实项目相同)
#if defined(_WIN32)
#define MYLIB_API __declspec(dllexport)
#elif defined(__GNUC__)
#define MYLIB_API __attribute__((visibility("default")))
#else
#define MYLIB_API
#endif

// dllexport 符号必须有外部链接，不能放在匿名命名空间内
namespace learn_demo_dll {
DEMO_API int demo_exported_add(int a, int b) {
    return a + b;
}

struct DEMO_API DemoExportedClass {
    int n = 0;
    int get() const { return n; }
    void set(int v) { n = v; }
};

MYLIB_API const char* mylib_version() {
    return "demo-1";
}
}  // namespace learn_demo_dll

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G2 declspec(dllexport) ===\n";

    assert(learn_demo_dll::demo_exported_add(2, 3) == 5);
    learn_demo_dll::DemoExportedClass c;
    c.set(9);
    assert(c.get() == 9);
    assert(std::string(learn_demo_dll::mylib_version()) == "demo-1");

#if defined(_WIN32)
    std::cout << "  Windows: export when building DLL, import when consuming\n";
#else
    std::cout << "  non-Windows: macro falls back to visibility/default\n";
#endif
    std::cout << "  .def files / module.modulemap are alternatives\n";
    std::cout << "  note: dllexport needs external linkage (named ns)\n";
    std::cout << "declspec_dllexport: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/declspec_dllexport", run>;

}  // namespace
