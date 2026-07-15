// Topic    : 拷贝构造 —— 深拷贝分配新资源，源对象不变
// Doc      : 第2部分-阶段5 · 步骤 1.1–1.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : copy_construction
// Topic id : part2/stage05/section01/copy_construction
// Refs     : https://en.cppreference.com/w/cpp/language/copy_constructor
//            ISO [class.copy.ctor]；Effective Modern C++ Item 23–25

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

namespace {

// 管理动态数组的资源类：演示「拷贝 = 分配新内存 + 复制内容」。
class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    explicit Buffer(std::size_t n) : data_(new int[n]{}), size_(n) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = static_cast<int>(i + 1);
        }
    }

    ~Buffer() { delete[] data_; }

    // 拷贝构造：深拷贝 —— 分配【新】内存，复制内容；源不变。
    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  [Buffer] copy-ctor: allocated " << size_ << " ints + memcpy\n";
    }

    // 本 topic 只讲拷贝构造；其余特殊成员按 Rule of 5 补齐，避免误用默认。
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

    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        return *this;
    }

    void swap(Buffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    std::size_t size() const { return size_; }
    const int* data() const { return data_; }
    int& operator[](std::size_t i) { return data_[i]; }
    int operator[](std::size_t i) const { return data_[i]; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [copy_construction] 入门：独立副本 ===\n";

    Buffer a(4);
    assert(a.size() == 4 && a[0] == 1 && a[3] == 4);
    std::cout << "a.size=" << a.size() << " a[0]=" << a[0] << '\n';

    // 拷贝构造：b 是 a 的深拷贝，内存地址不同
    Buffer b = a;
    assert(b.size() == a.size());
    assert(b.data() != a.data());  // 两块独立堆内存
    assert(b[0] == 1 && b[3] == 4);

    // 修改 b 不影响 a（独立所有权）
    b[0] = 99;
    assert(a[0] == 1);
    assert(b[0] == 99);
    std::cout << "after b[0]=99: a[0]=" << a[0] << " b[0]=" << b[0] << " (independent)\n";

    std::cout << "=== 进阶：std::string 同理；按值传参也会拷贝 ===\n";
    {
        std::string s = "payload";
        std::string t = s;  // string 的拷贝构造
        assert(s == "payload" && t == "payload");
        t[0] = 'P';
        assert(s == "payload" && t == "Payload");
        std::cout << "string copy: s=[" << s << "] t=[" << t << "]\n";
    }

    // 按值传参触发拷贝构造
    auto consume_by_value = [](Buffer local) {
        std::cout << "  consume_by_value got size=" << local.size() << '\n';
        assert(local.size() == 4);
    };
    consume_by_value(a);    // 再一次拷贝构造
    assert(a.size() == 4);  // a 仍完整

    std::cout << "=== 专家：何时写自定义拷贝构造 ===\n";
    // · 类管理原始资源（new[]/file/socket）→ 必须深拷贝或 =delete 拷贝
    // · Rule of 0：成员全是 string/vector/unique_ptr 时，编译器生成的拷贝通常已正确
    // · 拷贝可能很贵（大缓冲）；能移动就移动（见同 section 其它 topic）
    // · 若设计为独占资源，应 =delete 拷贝构造，只保留移动

    std::cout << "=== copy_construction: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/copy_construction", run>;

}  // namespace
