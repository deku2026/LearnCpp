// Topic    : 悬垂引用 / 悬垂指针 —— 识别与安全写法（不触发 UAF）
// Doc      : 第2部分-阶段5 · 步骤 11.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : dangling_reference_pointer
// Topic id : part2/stage05/section05/dangling_reference_pointer
// Refs     : https://en.cppreference.com/w/cpp/language/reference
//            ASan；C++ Core Guidelines

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

// ❌ 反面：返回局部变量的引用（切勿调用返回值）
const std::string& bad_return_local_ref() {
    static int once = 0;
    ++once;
    // 演示「模式」但不返回真正局部的可用引用：
    // 真实坏代码: std::string local="oops"; return local;
    // 这里返回 static 以避免本 topic 真的 UB：
    static const std::string keep = "static-ok";
    (void)once;
    return keep;
}

// ✅ 按值返回
std::string good_return_value() {
    std::string local = "ok";
    return local;  // NRVO / 移动
}

// ✅ 返回调用方拥有的缓冲区中的引用（生命周期由调用方保证）
const std::string& pick(const std::vector<std::string>& v, std::size_t i) {
    return v.at(i);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [dangling_reference_pointer] 入门：典型来源 ===\n";
    // 1) 返回局部变量的引用/指针
    // 2) 容器 reallocation 使迭代器/引用/指针失效
    // 3) 指向已 delete / 已离开作用域对象
    // 本文件只演示安全对照，不执行 use-after-free

    std::string s = good_return_value();
    assert(s == "ok");
    std::cout << "good_return_value -> " << s << '\n';

    const std::string& safe = bad_return_local_ref();  // 实际指向 static
    assert(safe == "static-ok");
    std::cout << "pattern note: never return ref to function-local automatic object\n";

    std::cout << "=== 进阶：容器失效（避免使用悬垂） ===\n";
    {
        std::vector<int> v{1, 2, 3};
        int* p = &v[0];
        const int old0 = *p;
        assert(old0 == 1);

        // push_back 可能扩容 → 旧指针失效
        v.reserve(v.capacity() + 1);  // 强制可能重分配的准备
        v.push_back(4);
        // 不读取旧 p！改为用下标/迭代器重新获取
        assert(v[0] == 1);
        assert(v.back() == 4);
        std::cout << "after growth: use fresh indices, not old pointers\n";
        (void)p;
    }

    std::cout << "=== 进阶：堆对象与智能指针 ===\n";
    {
        auto owner = std::make_unique<int>(5);
        int* raw = owner.get();
        assert(*raw == 5);
        owner.reset();  // 释放
        // *raw;  // UAF —— 禁止
        raw = nullptr;  // 防御性清空观察指针
        assert(raw == nullptr);
        std::cout << "after reset: null out borrow pointer; do not dereference\n";
    }

    std::cout << "=== 进阶：合法借用窗口 ===\n";
    {
        std::vector<std::string> names{"x", "y"};
        const std::string& r = pick(names, 1);
        assert(r == "y");
        // names 仍存活 → r 安全
        std::cout << "borrow from living container: " << r << '\n';
    }

    std::cout << "=== 专家：工具 ===\n";
    // · ASan：检测 heap-use-after-free / stack-use-after-return
    // · 返回 string/vector 用按值；共享所有权用 shared_ptr
    // · 观察指针/引用的生命周期必须 ≤ 被观察对象

    std::cout << "=== dangling_reference_pointer: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/dangling_reference_pointer", run>;

}  // namespace
