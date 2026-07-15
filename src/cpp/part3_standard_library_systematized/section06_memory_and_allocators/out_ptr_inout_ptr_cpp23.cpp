// Topic    : std::out_ptr / inout_ptr（C++23 对接 C 的 T**）
// Doc      : 第3部分-标准库系统化.md · 库 6.1
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : out_ptr_inout_ptr_cpp23
// Topic id : part3/section06/out_ptr_inout_ptr_cpp23
// Refs     : https://en.cppreference.com/w/cpp/memory/out_ptr_t
//            P1132；__cpp_lib_out_ptr

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

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
    delete *io_slot;
    *io_slot = new Foo{99};
    return 0;
}

void c_destroy(Foo* p) {
    delete p;
}

struct FooDeleter {
    void operator()(Foo* p) const noexcept { c_destroy(p); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [out_ptr_inout_ptr_cpp23] ===\n";

#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    std::cout << "__cpp_lib_out_ptr=" << __cpp_lib_out_ptr << '\n';

    std::unique_ptr<Foo, FooDeleter> p;
    [[maybe_unused]] int rc = c_create(std::out_ptr(p));
    assert(rc == 0 && p && p->id == 42);
    std::cout << "out_ptr create id=" << p->id << '\n';

    {
        std::unique_ptr<Foo, FooDeleter> q(new Foo{1});
        rc = c_replace(std::inout_ptr(q));
        assert(rc == 0 && q && q->id == 99);
        std::cout << "inout_ptr replace id=" << q->id << '\n';
    }

    {
        std::shared_ptr<int> s;
        auto create_int = [](int** out) -> int {
            *out = new int(7);
            return 0;
        };
        rc = create_int(std::out_ptr(s, std::default_delete<int>{}));
        assert(rc == 0 && s && *s == 7);
        std::cout << "shared_ptr out_ptr *s=" << *s << '\n';
    }

    std::cout << "out_ptr: overwrite smart pointer from C T** output\n";
    std::cout << "inout_ptr: hand current pointer to C (realloc-style)\n";
#else
    std::cout << "out_ptr feature missing; manual adopt path\n";
    std::unique_ptr<Foo, FooDeleter> p;
    Foo* raw = nullptr;
    int rc = c_create(&raw);
    assert(rc == 0 && raw);
    p.reset(raw);
    assert(p->id == 42);
    std::cout << "manual adopt id=" << p->id << '\n';
#endif

    std::cout << "[out_ptr_inout_ptr_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/out_ptr_inout_ptr_cpp23", run>;

}  // namespace
