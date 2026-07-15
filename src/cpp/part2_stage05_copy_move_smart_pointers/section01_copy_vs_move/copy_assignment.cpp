// Topic    : 拷贝赋值 —— 释放旧资源、复制新资源；自赋值安全
// Doc      : 第2部分-阶段5 · 步骤 1.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : copy_assignment
// Topic id : part2/stage05/section01/copy_assignment
// Refs     : https://en.cppreference.com/w/cpp/language/copy_assignment
//            ISO [class.copy.assign]；copy-and-swap idiom

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

namespace {

class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    Buffer() = default;
    explicit Buffer(std::size_t n, int fill = 0) : data_(new int[n]), size_(n) {
        std::fill(data_, data_ + size_, fill);
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  [Buffer] copy-ctor size=" << size_ << '\n';
    }

    Buffer(Buffer&& other) noexcept : data_(std::exchange(other.data_, nullptr)), size_(std::exchange(other.size_, 0)) {
        std::cout << "  [Buffer] move-ctor size=" << size_ << '\n';
    }

    // 写法 A：经典「先拷贝后释放」+ 自赋值检查
    Buffer& assign_classic(const Buffer& other) {
        if (this == &other) {
            return *this;  // 自赋值：直接返回，避免 delete 自己的 data_
        }
        // 先分配成功，再释放旧资源 → 强异常安全（分配失败时 *this 不变）
        int* fresh = new int[other.size_];
        std::copy(other.data_, other.data_ + other.size_, fresh);
        delete[] data_;
        data_ = fresh;
        size_ = other.size_;
        return *this;
    }

    // 写法 B：copy-and-swap —— 按值传参同时覆盖拷贝赋值与移动赋值
    Buffer& operator=(Buffer other) noexcept {
        // other 是左值时走拷贝构造；是右值时走移动构造
        swap(other);  // 旧资源随 other 析构释放
        return *this;
    }

    void swap(Buffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    std::size_t size() const { return size_; }
    int at(std::size_t i) const { return data_[i]; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int* data() const { return data_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [copy_assignment] 入门：独立赋值 ===\n";

    Buffer a(3, 7);
    Buffer b(2, 1);
    assert(a.size() == 3 && b.size() == 2);

    b = a;  // 拷贝赋值：b 变为 a 的独立副本
    assert(b.size() == 3);
    assert(b.at(0) == 7 && b.at(2) == 7);
    assert(b.data() != a.data());

    b[0] = 100;
    assert(a.at(0) == 7);  // a 不受影响
    std::cout << "after b=a then b[0]=100: a[0]=" << a.at(0) << " b[0]=" << b.at(0) << '\n';

    std::cout << "=== 进阶：自赋值安全 ===\n";
    {
        Buffer self(2, 5);
        Buffer* p = &self;
        self = *p;  // 自赋值：copy-and-swap 天然安全
        assert(self.size() == 2 && self.at(0) == 5);
        std::cout << "self-assignment via operator= OK\n";

        Buffer classic(2, 9);
        classic.assign_classic(classic);  // 显式自赋值检查路径
        assert(classic.size() == 2 && classic.at(0) == 9);
        std::cout << "self-assignment via assign_classic OK\n";
    }

    std::cout << "=== 进阶：string / 链式赋值 ===\n";
    {
        std::string x = "alpha";
        std::string y = "beta";
        std::string z = "gamma";
        x = y = z;  // 链式：从右到左
        assert(x == "gamma" && y == "gamma" && z == "gamma");
        std::cout << "chained string assignment: x=y=z -> " << x << '\n';
    }

    std::cout << "=== 专家：copy-and-swap 的取舍 ===\n";
    // · 优点：异常安全、自赋值安全、拷贝/移动赋值合一
    // · 代价：总是先构造临时再 swap；对「已有足够容量可复用」的类型可能多一次分配
    // · 手写拷贝赋值若先 delete 再 new：分配失败会丢资源（弱异常安全）
    // · 资源类优先考虑 Rule of 0（成员用 string/vector），少手写赋值

    Buffer c(1, 0);
    c = Buffer(4, 3);  // 临时右值 → 移动进 operator= 的 by-value 参数
    assert(c.size() == 4 && c.at(0) == 3);
    std::cout << "move into by-value operator=: c.size=" << c.size() << '\n';

    std::cout << "=== copy_assignment: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/copy_assignment", run>;

}  // namespace
