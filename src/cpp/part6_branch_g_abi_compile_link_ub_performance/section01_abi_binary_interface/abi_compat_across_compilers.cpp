// LearnCpp topic
// Doc      : 第6部分-支线G · G3 ABI 兼容
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : abi_compat_across_compilers
// Topic id : part6/g/section01/abi_compat_across_compilers
//
// 要点: 不同编译器/STL 的 C++ ABI 不通用; 跨边界用 C 或稳定 POD。
// 参考: libstdc++ dual ABI; Itanium vs MSVC

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

namespace {

// 不稳定: 在 DLL 边界传递 std::string / 虚类
// 稳定 C API:
extern "C" struct StablePoint {
    std::int32_t x;
    std::int32_t y;
};

extern "C" StablePoint stable_add(StablePoint a, StablePoint b) {
    return StablePoint{a.x + b.x, a.y + b.y};
}

// pimpl: 头文件大小固定
class Widget {
public:
    Widget();
    ~Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;
    int value() const;
    void set(int v);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

struct Widget::Impl {
    int value = 0;
    std::string name = "w";  // 实现细节变化不影响 Widget 大小
};

Widget::Widget() : impl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;
int Widget::value() const {
    return impl_->value;
}
void Widget::set(int v) {
    impl_->value = v;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G3 ABI compat across compilers ===\n";

    auto s = stable_add({1, 2}, {3, 4});
    assert(s.x == 4 && s.y == 6);

    Widget w;
    w.set(42);
    assert(w.value() == 42);
    std::cout << "  sizeof(Widget)=" << sizeof(Widget) << " (pointer-sized pimpl)\n";

    std::cout << "  do NOT link MSVC objects with MinGW C++ objects casually\n";
    std::cout << "  do NOT pass std::string across compiler/STL mismatch\n";
    std::cout << "  _GLIBCXX_USE_CXX11_ABI may change string layout\n";
    std::cout << "abi_compat_across_compilers: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/abi_compat_across_compilers", run>;

}  // namespace
