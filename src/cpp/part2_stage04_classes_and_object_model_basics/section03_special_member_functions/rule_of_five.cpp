// Topic     : Rule of 5
// Doc       : 第2部分-阶段4 · 步骤 3.5
// cppreference: https://en.cppreference.com/cpp/language/rule_of_three
//
// 要点: C.21——定义或 delete 任一特殊成员，就处理全部五个（析构+拷贝+移动）；
//       裸资源类完整实现；优先仍是 Rule of 0。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>
#include <utility>

namespace {

class IntBuffer {
    std::size_t n_ = 0;
    int* data_ = nullptr;

public:
    IntBuffer() = default;

    explicit IntBuffer(std::size_t n, int fill = 0) : n_(n), data_(n ? new int[n] : nullptr) {
        for (std::size_t i = 0; i < n_; ++i) {
            data_[i] = fill;
        }
    }

    // 1 析构
    ~IntBuffer() { delete[] data_; }

    // 2 拷贝构造
    IntBuffer(const IntBuffer& o) : n_(o.n_), data_(o.n_ ? new int[o.n_] : nullptr) {
        for (std::size_t i = 0; i < n_; ++i) {
            data_[i] = o.data_[i];
        }
    }

    // 3 拷贝赋值
    IntBuffer& operator=(const IntBuffer& o) {
        if (this != &o) {
            IntBuffer tmp(o);
            swap(tmp);
        }
        return *this;
    }

    // 4 移动构造
    IntBuffer(IntBuffer&& o) noexcept : n_(std::exchange(o.n_, 0)), data_(std::exchange(o.data_, nullptr)) {}

    // 5 移动赋值
    IntBuffer& operator=(IntBuffer&& o) noexcept {
        if (this != &o) {
            delete[] data_;
            n_ = std::exchange(o.n_, 0);
            data_ = std::exchange(o.data_, nullptr);
        }
        return *this;
    }

    void swap(IntBuffer& o) noexcept {
        std::swap(n_, o.n_);
        std::swap(data_, o.data_);
    }

    std::size_t size() const { return n_; }
    int operator[](std::size_t i) const { return data_[i]; }
};

// 五件套另一种常见形态：只移动（删拷贝）
class FileToken {
    int fd_ = -1;

public:
    explicit FileToken(int fd) : fd_(fd) {}
    ~FileToken() { fd_ = -1; /* close(fd_) */ }

    FileToken(const FileToken&) = delete;
    FileToken& operator=(const FileToken&) = delete;

    FileToken(FileToken&& o) noexcept : fd_(std::exchange(o.fd_, -1)) {}
    FileToken& operator=(FileToken&& o) noexcept {
        if (this != &o) {
            fd_ = std::exchange(o.fd_, -1);
        }
        return *this;
    }

    int fd() const { return fd_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [rule_of_five] ===\n";

    // -------------------------------------------------------------------------
    // §入门：五件套拷贝/移动
    // -------------------------------------------------------------------------
    IntBuffer a{3, 11};
    IntBuffer b{a};  // copy
    assert(b.size() == 3 && b[0] == 11);
    assert(a.size() == 3 && a[0] == 11);  // 深拷贝后双方独立且都有效

    IntBuffer c{std::move(a)};
    assert(c.size() == 3 && a.size() == 0);
    std::cout << "[intro] copy+move sizes b=" << b.size() << " c=" << c.size() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：赋值路径
    // -------------------------------------------------------------------------
    IntBuffer d{1, 5};
    d = b;
    assert(d.size() == 3 && d[1] == 11);
    IntBuffer e{2, 0};
    e = std::move(d);
    assert(e.size() == 3 && d.size() == 0);
    std::cout << "[advanced] copy/move assign ok\n";

    // -------------------------------------------------------------------------
    // §专家：只移动形态 + Rule of 0 对照
    // -------------------------------------------------------------------------
    FileToken f1{42};
    FileToken f2{std::move(f1)};
    assert(f2.fd() == 42);
    assert(f1.fd() == -1);
    static_assert(!std::is_copy_constructible_v<FileToken>);
    static_assert(std::is_move_constructible_v<FileToken>);

    // 首选仍是 Rule of 0：unique_ptr<int[]> 等托管，不必手写五件套
    std::cout << "[expert] move-only FileToken fd=" << f2.fd() << '\n';
    std::cout << "=== rule_of_five: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/rule_of_five", run>;

}  // namespace
