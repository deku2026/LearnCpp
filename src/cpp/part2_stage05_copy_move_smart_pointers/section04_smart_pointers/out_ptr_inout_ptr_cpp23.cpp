// Topic    : C++23 out_ptr / inout_ptr —— 智能指针对接 C 风格 T** 出参
// Doc      : 第2部分-阶段5 · 步骤 9.6
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : out_ptr_inout_ptr_cpp23
// Topic id : part2/stage05/section04/out_ptr_inout_ptr_cpp23
// Refs     : https://en.cppreference.com/w/cpp/memory/out_ptr_t
//            https://en.cppreference.com/w/cpp/memory/inout_ptr_t
//            P1132；__cpp_lib_out_ptr

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <version>

namespace {

// 模拟 C API：通过 T** 输出新对象
struct Foo {
    int id;
};

int c_create(Foo** out) {
    if (!out) {
        return -1;
    }
    *out = new Foo{42};
    return 0;
}

int c_replace(Foo** io_slot) {
    if (!io_slot) {
        return -1;
    }
    delete *io_slot;  // 旧对象由 C API 释放（示意）
    *io_slot = new Foo{99};
    return 0;
}

void c_destroy(Foo* p) {
    delete p;
}

struct FooDeleter {
    void operator()(Foo* p) const noexcept { c_destroy(p); }
};

#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [out_ptr_inout_ptr_cpp23] 入门：out_ptr ===\n";
    std::cout << "__cpp_lib_out_ptr=" << __cpp_lib_out_ptr << '\n';

    std::unique_ptr<Foo, FooDeleter> p;
    int rc = c_create(std::out_ptr(p));
    assert(rc == 0);
    assert(p && p->id == 42);
    std::cout << "after c_create via out_ptr: id=" << p->id << '\n';

    std::cout << "=== 进阶：inout_ptr（替换已有托管指针） ===\n";
    {
        std::unique_ptr<Foo, FooDeleter> q(new Foo{1});
        rc = c_replace(std::inout_ptr(q));
        assert(rc == 0);
        assert(q && q->id == 99);
        std::cout << "after c_replace via inout_ptr: id=" << q->id << '\n';
    }

    std::cout << "=== 进阶：shared_ptr + out_ptr（必须带 deleter 实参） ===\n";
    {
        // 标准要求：out_ptr(shared_ptr) 必须提供删除器（不能依赖默认推断）
        std::shared_ptr<int> s;
        auto create_int = [](int** out) -> int {
            *out = new int(7);
            return 0;
        };
        rc = create_int(std::out_ptr(s, std::default_delete<int>{}));
        assert(rc == 0 && s && *s == 7);
        std::cout << "shared_ptr out_ptr *s=" << *s << '\n';
    }

    std::cout << "=== 专家：out vs in-out ===\n";
    // · out_ptr：假定智能指针当前值将被覆盖；通常先释放旧资源再写入新指针
    // · inout_ptr：把当前指针交给 C API，由 API 可能释放/替换
    // · 消除「Foo* raw{}; api(&raw); p.reset(raw);」样板与泄漏窗口
    // · 与第 8 部分 FFI 对接 C 库同一模式

    std::cout << "=== out_ptr_inout_ptr_cpp23: OK ===\n";
    return 0;
}

#else

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [out_ptr_inout_ptr_cpp23] feature not available; manual path ===\n";
    std::unique_ptr<Foo, FooDeleter> p;
    Foo* raw = nullptr;
    int rc = c_create(&raw);
    assert(rc == 0 && raw);
    p.reset(raw);
    assert(p->id == 42);
    std::cout << "manual adopt id=" << p->id << '\n';
    return 0;
}

#endif

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/out_ptr_inout_ptr_cpp23", run>;

}  // namespace
