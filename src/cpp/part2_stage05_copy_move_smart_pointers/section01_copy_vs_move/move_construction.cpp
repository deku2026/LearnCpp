// Topic    : 移动构造 —— O(1) 接管资源并把源置空（防双重释放）
// Doc      : 第2部分-阶段5 · 步骤 1.2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : move_construction
// Topic id : part2/stage05/section01/move_construction
// Refs     : https://en.cppreference.com/w/cpp/language/move_constructor
//            ISO [class.copy.ctor]；Effective Modern C++ Item 23–25

#include "learn/topic_registry.hpp"

#include <algorithm>
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
    explicit Buffer(std::size_t n) : data_(new int[n]{}), size_(n) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = static_cast<int>(i);
        }
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  [Buffer] copy-ctor (expensive)\n";
    }

    // 移动构造三步：① 接管指针 ② 源置空 ③ 源可安全析构
    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // 关键：否则 other 析构会 double-free
        other.size_ = 0;
        std::cout << "  [Buffer] move-ctor O(1) steal ownership\n";
    }

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
    int at(std::size_t i) const { return data_[i]; }
};

// 具名右值引用参数 other 作为表达式是 lvalue —— 成员仍需 std::move
struct Wrapper {
    std::string s;
    Buffer buf;

    Wrapper(std::string ss, std::size_t n) : s(std::move(ss)), buf(n) {}

    Wrapper(Wrapper&& other) noexcept
        : s(std::move(other.s)),  // 必须 move；写成 other.s 会拷贝 string
          buf(std::move(other.buf)) {}

    // Rule of 5 简配：本 demo 只触发移动构造路径
    Wrapper(const Wrapper&) = delete;
    Wrapper& operator=(const Wrapper&) = delete;
    Wrapper& operator=(Wrapper&&) = delete;
    ~Wrapper() = default;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [move_construction] 入门：偷资源 ===\n";

    Buffer a(1000);
    [[maybe_unused]] const int* raw_a = a.data();
    assert(a.size() == 1000 && a.at(0) == 0 && a.at(999) == 999);

    Buffer c = std::move(a);  // 移动构造：O(1)
    assert(c.size() == 1000);
    assert(c.data() == raw_a);  // 同一块内存被接管
    assert(a.size() == 0);      // 源已置空
    assert(a.data() == nullptr);
    assert(c.at(999) == 999);
    std::cout << "a.size=" << a.size() << " c.size=" << c.size() << " (same buffer pointer stolen)\n";

    std::cout << "=== 进阶：string / vector 移动；具名 && 仍是 lvalue ===\n";
    {
        std::string s = "long-payload................";
        std::string t = std::move(s);
        assert(t.find("long-payload") == 0);
        // s 处于 valid-but-unspecified；常见实现为空，但不依赖具体值
        std::cout << "string moved: t.size=" << t.size() << " s.empty()=" << s.empty() << '\n';

        std::vector<int> v{1, 2, 3, 4};
        [[maybe_unused]] const int* pv = v.data();
        std::vector<int> w = std::move(v);
        assert(w.data() == pv && w.size() == 4);
        assert(v.empty());  // vector 移动后通常为空且合法
        std::cout << "vector moved: w.size=" << w.size() << " v.empty()=" << v.empty() << '\n';
    }

    {
        Wrapper w1{"hello", 8};
        Wrapper w2 = std::move(w1);
        assert(w2.s == "hello");
        assert(w2.buf.size() == 8);
        // w1.s / w1.buf 已 moved-from，只做无前置条件检查
        assert(w1.buf.size() == 0);
        std::cout << "Wrapper member-wise move: w2.s=" << w2.s << " w1.buf.size=" << w1.buf.size() << '\n';
    }

    std::cout << "=== 专家：noexcept、回退拷贝、何时 delete 移动 ===\n";
    // · 移动构造应标 noexcept（vector 扩容依赖，见 noexcept_move_and_vector_growth）
    // · 若类型只有拷贝、没有移动，std::move(x) 会退化为拷贝（右值可绑 const T&）
    // · 不可移动类型（mutex、iostream 的某些包装）应 =delete 移动，或只放不动
    // · 移动后源必须可析构、可再赋值（valid state）

    static_assert(std::is_nothrow_move_constructible_v<Buffer>);
    static_assert(std::is_nothrow_move_constructible_v<Wrapper>);

    std::cout << "=== move_construction: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/move_construction", run>;

}  // namespace
