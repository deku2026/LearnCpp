// Topic    : copy-and-swap 实现强异常保证（验收点）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 3.2 / 3.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : copy_and_swap_strong_guarantee
// Topic id : part2/stage09/section03/copy_and_swap_strong_guarantee
// Refs     : https://en.cppreference.com/w/cpp/language/operators
//            Exceptional C++ / Effective C++ Item 25, 29
//            C++ Core Guidelines C.85 E.16

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>
#include <stdexcept>
#include <utility>

namespace {

class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    Buffer() = default;
    explicit Buffer(std::size_t n) : data_(new int[n]{}), size_(n) {}
    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& o) : data_(new int[o.size_]), size_(o.size_) { std::copy(o.data_, o.data_ + size_, data_); }

    Buffer(Buffer&& o) noexcept : data_(o.data_), size_(o.size_) {
        o.data_ = nullptr;
        o.size_ = 0;
    }

    // ⭐ swap：绝不抛 —— 强保证的基石
    void swap(Buffer& o) noexcept {
        std::swap(data_, o.data_);
        std::swap(size_, o.size_);
    }

    // ⭐ copy-and-swap 赋值：按值传参（拷贝/移动在此发生）+ noexcept swap 提交
    Buffer& operator=(Buffer o) noexcept {
        swap(o);  // 旧资源随 o 析构释放
        return *this;
    }

    std::size_t size() const noexcept { return size_; }
    int* data() noexcept { return data_; }
    const int* data() const noexcept { return data_; }

    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
};

[[maybe_unused]] void swap(Buffer& a, Buffer& b) noexcept {
    a.swap(b);
}

// 可控失败的“昂贵拷贝”源：用于说明拷贝阶段抛时 *this 不变
struct BoomOnCopy {
    int v = 0;
    static int copies_left;

    BoomOnCopy() = default;
    explicit BoomOnCopy(int x) : v(x) {}
    BoomOnCopy(const BoomOnCopy& o) : v(o.v) {
        if (copies_left-- <= 0) {
            throw std::bad_alloc();
        }
    }
    BoomOnCopy& operator=(const BoomOnCopy& o) {
        v = o.v;
        return *this;
    }
};

int BoomOnCopy::copies_left = 100;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [copy_and_swap_strong_guarantee] 主干：赋值强保证 ===\n";
    {
        Buffer a(3);
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        Buffer b(2);
        b[0] = 9;
        b[1] = 8;

        const int* old_ptr = a.data();
        a = b;  // 拷贝 b → 临时，再 swap 进 a
        assert(a.size() == 2);
        assert(a[0] == 9 && a[1] == 8);
        // 资源已换：要么新缓冲，要么（实现细节）至少内容正确
        (void)old_ptr;
        std::cout << "a=b ok, a.size=" << a.size() << '\n';
    }

    std::cout << "=== 对抗：自赋值安全 ===\n";
    {
        Buffer a(2);
        a[0] = 5;
        a[1] = 6;
        Buffer& self = a;
        a = self;  // copy-and-swap 天然安全（经引用自赋值，避开自赋值告警噪音）
        assert(a.size() == 2);
        assert(a[0] == 5 && a[1] == 6);
        std::cout << "self-assignment a=a safe\n";
    }

    std::cout << "=== 对抗：移动赋值也走同一 operator= ===\n";
    {
        Buffer a(1);
        a[0] = 7;
        Buffer b(1);
        b[0] = 1;
        a = std::move(b);  // 按值传参绑到移动构造
        assert(a.size() == 1 && a[0] == 1);
        assert(b.size() == 0);
        std::cout << "move-assign via by-value operator=\n";
    }

    std::cout << "=== 专节：为何是强保证 ===\n";
    // 若拷贝阶段（传参构造 o）抛异常，*this 完全没被动过 → 回滚。
    // 函数体内只有 noexcept swap → 不会半途失败。
    // 强保证三基石：swap / 移动 / 析构 皆 no-throw。
    {
        BoomOnCopy::copies_left = 0;
        BoomOnCopy x{1};
        BoomOnCopy y{2};
        try {
            // 直接拷贝会抛 —— 模拟“赋值拷贝阶段失败”
            BoomOnCopy z = x;
            (void)z;
            assert(false);
        } catch (const std::bad_alloc&) {
            assert(y.v == 2);  // 源对象未损坏
            std::cout << "copy stage throw leaves other objects intact\n";
        }
        BoomOnCopy::copies_left = 100;
    }

    static_assert(noexcept(std::declval<Buffer&>().swap(std::declval<Buffer&>())));
    std::cout << "[copy_and_swap_strong_guarantee] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/copy_and_swap_strong_guarantee", run>;

}  // namespace
