// Topic    : 移动赋值 —— 释放己方旧资源，接管源；自移动保持有效状态
// Doc      : 第2部分-阶段5 · 步骤 1.2–1.3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : move_assignment
// Topic id : part2/stage05/section01/move_assignment
// Refs     : https://en.cppreference.com/w/cpp/language/move_assignment
//            ISO [class.copy.assign] / [lib.types.movedfrom]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    Buffer() = default;
    explicit Buffer(std::size_t n, int fill = 0) : data_(new int[n]), size_(n) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = fill;
        }
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)), size_(std::exchange(other.size_, 0)) {}

    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;
        }
        Buffer tmp(other);
        swap(tmp);
        return *this;
    }

    // 移动赋值：释放 *this 旧资源 → 接管 other → other 置空
    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) {
            // 自移动：标准库保证 valid-but-unspecified；此处选择 no-op 更稳妥
            return *this;
        }
        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        std::cout << "  [Buffer] move-assign size=" << size_ << '\n';
        return *this;
    }

    void swap(Buffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    std::size_t size() const { return size_; }
    int at(std::size_t i) const { return data_[i]; }
    const int* data() const { return data_; }
};

// 用 exchange + 成员 swap 的另一种惯用写法（不先 delete，靠 tmp 析构）
class BufferExchange {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    explicit BufferExchange(std::size_t n) : data_(new int[n]{}), size_(n) {}
    ~BufferExchange() { delete[] data_; }

    BufferExchange(BufferExchange&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)), size_(std::exchange(other.size_, 0)) {}

    BufferExchange& operator=(BufferExchange&& other) noexcept {
        // 先把 other 的资源换进来，自己的旧资源进 other，随 other 离开（或再被掏空）
        BufferExchange tmp(std::move(other));
        std::swap(data_, tmp.data_);
        std::swap(size_, tmp.size_);
        return *this;
    }

    BufferExchange(const BufferExchange&) = delete;
    BufferExchange& operator=(const BufferExchange&) = delete;

    std::size_t size() const { return size_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [move_assignment] 入门：接管并释放旧资源 ===\n";

    Buffer dst(2, 1);
    Buffer src(5, 9);
    [[maybe_unused]] const int* raw_src = src.data();

#if defined(__clang__)

#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Wself-assign-overloaded"

#pragma clang diagnostic ignored "-Wself-move"

#endif

    dst = std::move(src);

#if defined(__clang__)

#pragma clang diagnostic pop

#endif

    assert(dst.size() == 5);
    assert(dst.data() == raw_src);
    assert(dst.at(0) == 9);
    assert(src.size() == 0 && src.data() == nullptr);
    std::cout << "dst.size=" << dst.size() << " src.size=" << src.size() << '\n';

    std::cout << "=== 进阶：标准库类型与容器 ===\n";
    {
        std::string a = "keep-me";
        std::string b = "steal-me-please........";
        a = std::move(b);
        assert(a.find("steal") == 0);
        // b valid-but-unspecified：可 clear / 再赋值，不可假设内容
        b = "rebuilt";
        assert(b == "rebuilt");
        std::cout << "string move-assign then rebuild b=[" << b << "]\n";

        std::vector<std::string> v{"x", "y"};
        std::vector<std::string> u;
        u = std::move(v);
        assert(u.size() == 2 && u[0] == "x");
        assert(v.empty());
        std::cout << "vector move-assign: u.size=" << u.size() << " v.empty=" << v.empty() << '\n';
    }

    std::cout << "=== 进阶：自移动赋值 ===\n";
    {
        Buffer self(3, 4);
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-move"
#endif
        self = std::move(self);  // 必须不崩溃；值可能未指定
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
        // 只调用无前置条件操作
        self = Buffer(1, 2);  // 重新赋值恢复确定状态
        assert(self.size() == 1 && self.at(0) == 2);
        std::cout << "self move-assign then reassign OK\n";
    }

    {
        BufferExchange x(10);
        BufferExchange y(20);
        x = std::move(y);
        assert(x.size() == 20);
        assert(y.size() == 0);
        std::cout << "BufferExchange move-assign via temporary swap OK\n";
    }

    std::cout << "=== 专家：实现要点 ===\n";
    // · 移动赋值应 noexcept（与移动构造同理）
    // · 必须处理自赋值/自移动，或用 copy-and-swap / move-and-swap 天然安全
    // · 先释放 *this 再接管时，自移动若不检查会 delete 自己刚要接管的指针 → 灾难
    // · 源对象置空后应可析构、可赋值

    static_assert(std::is_nothrow_move_assignable_v<Buffer>);

    std::cout << "=== move_assignment: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/move_assignment", run>;

}  // namespace
