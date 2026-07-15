// Topic    : std::unreachable（C++23；误达 = UB）
// Doc      : 第3部分-标准库系统化.md · 库 7.2
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : std_unreachable_cpp23
// Topic id : part3/section07/std_unreachable_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/unreachable
//            P0627

#include "learn/topic_registry.hpp"

#include <iostream>
#include <utility>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

// 教学路径：合法输入用 switch；非法输入不真正调用 unreachable（避免 UB）
[[maybe_unused]] int classify_checked(int x) {
    switch (x) {
        case 0:
            return 100;
        case 1:
            return 200;
        default:
            // 真实生产若能证明 x 只能是 0/1：std::unreachable();
            // 学习/验收路径用 assert 挡住，保证 topic 本身不触发 UB
            assert(false && "precondition violated; would be std::unreachable()");
            return -1;
    }
}

[[maybe_unused]] int classify_documented(int x) {
    switch (x) {
        case 0:
            return 100;
        case 1:
            return 200;
        default:
#if defined(__cpp_lib_unreachable)
            // 仅当调用方保证穷举时使用；此处函数不对外暴露非法输入
            std::unreachable();
#else
            assert(false);
            return -1;
#endif
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_unreachable_cpp23] 合法路径 ===\n";
#if defined(__cpp_lib_unreachable)
    std::cout << "__cpp_lib_unreachable=" << __cpp_lib_unreachable << '\n';
#endif

    assert(classify_checked(0) == 100);
    assert(classify_checked(1) == 200);
    assert(classify_documented(0) == 100);
    assert(classify_documented(1) == 200);
    std::cout << "classify 0/1 -> 100/200\n";

    std::cout << "=== 语义（不触发） ===\n";
    std::cout << "std::unreachable() = optimizer contract: path never taken\n";
    std::cout << "if taken -> undefined behavior (may fall through / delete code)\n";
    std::cout << "prefer: exhaust enums, or return error instead of unreachable\n";

    // 确保符号可链接（不调用）
    (void)static_cast<void (*)()>(std::unreachable);

    std::cout << "[std_unreachable_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/std_unreachable_cpp23", run>;

}  // namespace
