// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.1 ASan
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : asan_overview
// Topic id : part4/section05/asan_overview
//
// 要点: 抓堆/栈/全局越界、UAF、double-free。Windows: /fsanitize=address
// 仓库: cmake/Sanitizers.cmake LEARNCPP_ENABLE_ASAN
// 纪律: 演示安全边界写法; 不故意在进程里触发溢出(避免本机 ASan 杀进程)。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

namespace {

bool asan_built_in() {
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
    return true;
#endif
#endif
#if defined(__SANITIZE_ADDRESS__)
    return true;
#endif
    return false;
}

// 安全 API: 拒绝越界
[[maybe_unused]] bool heap_write(std::vector<int>& buf, std::size_t i, int v) {
    if (i >= buf.size()) {
        return false;  // ASan 会抓的是 unchecked buf[i]=v 当 i 越界
    }
    buf[i] = v;
    return true;
}

// UAF 的正确替代: unique_ptr 生命周期
[[maybe_unused]] int use_after_free_safe() {
    auto p = std::make_unique<int>(7);
    int v = *p;
    p.reset();
    // 不再解引用 p
    return v;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== asan_overview ===\n";
    std::cout << "  __SANITIZE_ADDRESS__/feature: " << (asan_built_in() ? "ON" : "OFF") << '\n';

    std::vector<int> buf(3);
    assert(heap_write(buf, 0, 1));
    assert(heap_write(buf, 2, 3));
    assert(!heap_write(buf, 5, 9));  // 越界被拒绝(若写成 buf[5] ASan 会报)
    std::cout << "  heap OOB write rejected by bounds check\n";

    assert(use_after_free_safe() == 7);
    std::cout << "  prefer unique_ptr over raw new/delete\n";

    // 旗标对照
    // Windows clang-cl: /fsanitize=address /Zi
    // Linux: -fsanitize=address -fno-omit-frame-pointer -g
    [[maybe_unused]] const char* win = "/fsanitize=address";
    [[maybe_unused]] const char* nix = "-fsanitize=address";
    assert(std::string_view(win).find("address") != std::string_view::npos);
    assert(std::string_view(nix).find("address") != std::string_view::npos);

    // 仓库入口
    std::cout << "  CMake: LEARNCPP_ENABLE_ASAN -> Sanitizers.cmake\n";
    std::cout << "asan_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/asan_overview", run>;

}  // namespace
