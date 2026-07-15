// Topic    : unique_ptr 自定义删除器 —— 管理 FILE* / 任意获取-释放对
// Doc      : 第2部分-阶段5 · 步骤 9.3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : unique_ptr_custom_deleter
// Topic id : part2/stage05/section04/unique_ptr_custom_deleter
// Refs     : https://en.cppreference.com/w/cpp/memory/unique_ptr
//            Effective Modern C++ Item 18

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace {

int g_fclose_calls = 0;
int g_free_handle = 0;

// 模拟 C API handle
struct CHandle {
    int id;
};

CHandle* c_open(int id) {
    return new CHandle{id};
}
void c_close(CHandle* h) {
    if (h) {
        ++g_free_handle;
        std::cout << "  c_close id=" << h->id << '\n';
        delete h;
    }
}

struct CHandleCloser {
    void operator()(CHandle* h) const noexcept { c_close(h); }
};

// 函数指针删除器：状态为空，但类型是函数指针 → 可能增加 unique_ptr 体积
void file_closer(std::FILE* f) {
    if (f) {
        ++g_fclose_calls;
        std::fclose(f);
        std::cout << "  fclose via function pointer\n";
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [unique_ptr_custom_deleter] 入门：FILE* ===\n";

    g_fclose_calls = 0;
    {
        // 临时文件路径：写完即关；演示删除器
        const char* path = "learncpp_stage05_unique_deleter.tmp";
        auto closer = [](std::FILE* f) {
            if (f) {
                ++g_fclose_calls;
                std::fclose(f);
                std::cout << "  fclose via lambda\n";
            }
        };
        std::unique_ptr<std::FILE, decltype(closer)> fp(std::fopen(path, "wb"), closer);
        if (fp) {
            const char msg[] = "hello";
            std::fwrite(msg, 1, sizeof(msg) - 1, fp.get());
        }
        assert(fp);
    }  // 离开作用域 → closer → fclose
    assert(g_fclose_calls == 1);

    // 清理临时文件（用标准 fstream 删除内容路径；失败忽略）
    std::remove("learncpp_stage05_unique_deleter.tmp");

    std::cout << "=== 进阶：C handle + 函数对象删除器 ===\n";
    {
        g_free_handle = 0;
        std::unique_ptr<CHandle, CHandleCloser> h(c_open(7));
        assert(h && h->id == 7);
        // 体积：空的函数对象删除器可 EBO，通常仍为指针大小
        std::cout << "sizeof unique_ptr with empty deleter=" << sizeof(h) << " sizeof(void*)=" << sizeof(void*) << '\n';
    }
    assert(g_free_handle == 1);

    std::cout << "=== 进阶：函数指针删除器 ===\n";
    {
        g_fclose_calls = 0;
        const char* path = "learncpp_stage05_unique_deleter2.tmp";
        std::unique_ptr<std::FILE, decltype(&file_closer)> fp(std::fopen(path, "wb"), &file_closer);
        if (fp) {
            std::fputs("x", fp.get());
        }
    }
    assert(g_fclose_calls == 1);
    std::remove("learncpp_stage05_unique_deleter2.tmp");

    std::cout << "=== 专家：删除器与类型 / 数组 ===\n";
    // · unique_ptr<T, D> 的 D 是类型的一部分 → 不同删除器 = 不同类型
    // · 有状态删除器会增大 sizeof(unique_ptr)
    // · unique_ptr<T[]> 默认 delete[]；自定义删除器同样可绑数组
    // · shared_ptr 删除器类型擦除，不进 shared_ptr 类型签名（对比）

    auto arr = std::unique_ptr<int[]>(new int[3]{1, 2, 3});
    assert(arr[0] == 1 && arr[2] == 3);

    std::cout << "=== unique_ptr_custom_deleter: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/unique_ptr_custom_deleter", run>;

}  // namespace
