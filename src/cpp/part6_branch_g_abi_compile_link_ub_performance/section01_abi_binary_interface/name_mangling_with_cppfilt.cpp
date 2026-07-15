// LearnCpp topic
// Doc      : 第6部分-支线G · G1 name mangling
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : name_mangling_with_cppfilt
// Topic id : part6/g/section01/name_mangling_with_cppfilt
//
// 要点: C++ 把重载/命名空间/模板编码进符号名; nm + c++filt / undname 解码。
// 参考: Itanium C++ ABI; [dcl.link]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <typeinfo>

namespace lib {

int compute(int x) {
    return x + 1;
}
int compute(int x, double y) {
    return x + static_cast<int>(y);
}

template <typename T>
T identity(T v) {
    return v;
}

// 强制实例化, 保证符号存在于目标文件 (教学)
template double identity<double>(double);

}  // namespace lib

extern "C" int g_c_style_symbol(int x) {
    return x;
}

namespace {

// 用编译器扩展取近似 mangled 名 (若可用)
std::string maybe_pretty(const char* raw) {
    return raw ? std::string(raw) : std::string{};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G1 name mangling / c++filt ===\n";

    assert(lib::compute(1) == 2);
    assert(lib::compute(1, 2.5) == 3);
    assert(lib::identity(3.5) == 3.5);
    assert(g_c_style_symbol(9) == 9);

#if defined(__GNUC__) || defined(__clang__)
    // Itanium: 展示 typeinfo/函数名
    std::cout << "  typeid(compute).name raw may be mangled: " << typeid(decltype(lib::compute(0))).name() << '\n';
    // abi::__cxa_demangle 需 cxxabi; 这里只说明工具链
    std::cout << "  try: nm <obj> | c++filt\n";
    std::cout << "  expect demangled like: lib::compute(int)\n";
#elif defined(_MSC_VER)
    std::cout << "  MSVC mangling differs (undname.exe)\n";
    std::cout << "  try: dumpbin /symbols <obj>\n";
#endif

    // extern "C" 不 mangle (C 链接)
    std::cout << "  extern \"C\" g_c_style_symbol → stable C name\n";
    std::cout << "  overload compute(int) vs compute(int,double) → different symbols\n";

    (void)maybe_pretty;
    std::cout << "name_mangling_with_cppfilt: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/name_mangling_with_cppfilt", run>;

}  // namespace
