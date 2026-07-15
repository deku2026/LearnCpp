// Topic    : 智能指针总览 unique_ptr / shared_ptr / weak_ptr
// Doc      : 第3部分-标准库系统化.md · 库 6.1
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : smart_pointers_overview
// Topic id : part3/section06/smart_pointers_overview
// Refs     : https://en.cppreference.com/w/cpp/memory
//            https://en.cppreference.com/w/cpp/memory/unique_ptr

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <iostream>
#include <memory>
#include <utility>

namespace {

struct Node {
    int id{};
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> back;  // 打破环
    explicit Node(int i) : id{i} { std::cout << "  Node(" << id << ") ctor\n"; }
    ~Node() { std::cout << "  Node(" << id << ") dtor\n"; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [smart_pointers_overview] unique_ptr 独占 ===\n";
    {
        auto p = std::make_unique<int>(42);
        assert(p && *p == 42);
        auto q = std::move(p);
        assert(!p && q && *q == 42);
        std::cout << "unique move *q=" << *q << '\n';
    }

    std::cout << "=== shared_ptr 共享 + use_count ===\n";
    {
        auto a = std::make_shared<int>(7);
        assert(a.use_count() == 1);
        {
            auto b = a;
            assert(a.use_count() == 2);
            std::cout << "use_count=" << a.use_count() << " *b=" << *b << '\n';
        }
        assert(a.use_count() == 1);
    }

    std::cout << "=== weak_ptr 破环 ===\n";
    {
        auto n1 = std::make_shared<Node>(1);
        auto n2 = std::make_shared<Node>(2);
        n1->next = n2;
        n2->back = n1;  // weak，不增加 use_count
        assert(n1.use_count() == 1);
        assert(n2.use_count() == 2);  // n2 + n1->next
        if (auto locked = n2->back.lock()) {
            assert(locked->id == 1);
            std::cout << "weak lock id=" << locked->id << '\n';
        }
        n1.reset();
        assert(n2->back.expired());
        std::cout << "after n1 reset, back expired\n";
    }

    std::cout << "=== 自定义删除器（对接 C API） ===\n";
    {
        auto closer = [](FILE* f) {
            if (f) {
                std::fclose(f);
                std::cout << "  FILE closed by deleter\n";
            }
        };
// 用 tmpfile 演示 RAII；失败则跳过
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdeprecated-enum-float-conversion"
#endif
        std::unique_ptr<FILE, decltype(closer)> fp{std::tmpfile(), closer};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
        if (fp) {
            std::fputs("hi", fp.get());
            std::cout << "tmpfile write ok\n";
        } else {
            std::cout << "tmpfile unavailable; skipped\n";
        }
    }

    std::cout << "[smart_pointers_overview] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/smart_pointers_overview", run>;

}  // namespace
