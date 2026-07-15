// Topic    : std::launder（C++17 少数底层场景）
// Doc      : 第3部分-标准库系统化.md · 库 6.4
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : std_launder
// Topic id : part3/section06/std_launder
// Refs     : https://en.cppreference.com/w/cpp/utility/launder
//            https://en.cppreference.com/w/cpp/language/lifetime

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <new>
#include <type_traits>

namespace {

struct Widget {
    const int id;  // const 成员：placement new 替换后，旧 glvalue 不可靠
    explicit Widget(int i) : id{i} {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_launder] placement new 重用存储 ===\n";

    alignas(Widget) unsigned char storage[sizeof(Widget)];
    auto* w1 = ::new (storage) Widget(1);
    assert(w1->id == 1);
    std::cout << "first Widget id=" << w1->id << '\n';

    // 结束旧对象生命周期，在同一存储构造新对象
    w1->~Widget();
    auto* w2 = ::new (storage) Widget(2);
    assert(w2->id == 2);

    // 通过“可能指向旧对象”的指针访问时，用 launder 获得指向新对象的指针
    Widget* laundered = std::launder(reinterpret_cast<Widget*>(storage));
    assert(laundered->id == 2);
    std::cout << "after placement new, laundered id=" << laundered->id << '\n';

    // 直接用 w2 也正确——launder 解决的是“编译器仍认为指针指向旧 const 对象”的情况
    assert(w2->id == 2);
    w2->~Widget();

    std::cout << "=== 何时需要 ===\n";
    std::cout << "  - placement new 替换含 const/引用成员的对象后，旧指针路径\n";
    std::cout << "  - 极少数与编译器别名假设相关的底层代码\n";
    std::cout << "  - 日常业务代码几乎不需要；优先新指针 / start_lifetime_as\n";

    // 探测：launder 对平凡类型可调用
    int x = 42;
    int* p = std::launder(&x);
    assert(*p == 42);
    std::cout << "launder(&int) trivial case *p=" << *p << '\n';

    std::cout << "[std_launder] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/std_launder", run>;

}  // namespace
