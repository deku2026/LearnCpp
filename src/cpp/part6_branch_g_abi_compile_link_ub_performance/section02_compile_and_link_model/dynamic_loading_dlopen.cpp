// LearnCpp topic
// Doc      : 第6部分-支线G · G5.3 动态加载
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : dynamic_loading_dlopen
// Topic id : part6/g/section02/dynamic_loading_dlopen
//
// 要点: dlopen/LoadLibrary 运行时加载; 符号用 extern "C" 最稳妥。
// 参考: POSIX dlopen; Windows LoadLibrary

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace {

// 插件风格 C API (本进程内函数模拟插件导出)
extern "C" int plugin_entry(int x) {
    return x * 3;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G5 dynamic loading (dlopen / LoadLibrary) ===\n";

    // 直接调用模拟
    assert(plugin_entry(7) == 21);

#if defined(_WIN32)
    // 加载系统库演示 API (不加载自己, 避免路径问题)
    HMODULE mod = LoadLibraryA("kernel32.dll");
    if (mod) {
        [[maybe_unused]] auto proc = reinterpret_cast<void*>(GetProcAddress(mod, "GetTickCount"));
        assert(proc != nullptr);
        FreeLibrary(mod);
        std::cout << "  LoadLibrary(kernel32) + GetProcAddress(GetTickCount) OK\n";
    } else {
        std::cout << "  LoadLibrary demo skipped\n";
    }
#else
    // 加载 libc 数学 (平台相关库名)
    void* h = dlopen("libm.so.6", RTLD_LAZY);
    if (!h) {
        h = dlopen("libm.so", RTLD_LAZY);
    }
    if (h) {
        auto sym = dlsym(h, "sqrt");
        assert(sym != nullptr);
        dlclose(h);
        std::cout << "  dlopen(libm) + dlsym(sqrt) OK\n";
    } else {
        std::cout << "  dlopen demo skipped: " << dlerror() << '\n';
    }
#endif

    std::cout << "  export plugin API as extern \"C\" for stable names\n";
    std::cout << "  mind lifetime: unload only when no code/data in use\n";
    std::cout << "dynamic_loading_dlopen: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/dynamic_loading_dlopen", run>;

}  // namespace
