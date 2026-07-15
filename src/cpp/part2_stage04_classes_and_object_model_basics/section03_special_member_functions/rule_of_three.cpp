// Topic     : Rule of 3
// Doc       : 第2部分-阶段4 · 步骤 3.5
// cppreference: https://en.cppreference.com/cpp/language/rule_of_three
//
// 要点: 手写析构/拷贝构造/拷贝赋值任一，通常三者都要写（协同管理同一裸资源）；
//       C++98 时代经典；C++11 起扩展为 Rule of 5。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

namespace {

// 裸 char* 缓冲区：Rule of 3 完整实现（故意不写移动，模拟 C++98 风格）
class CString {
    char* data_ = nullptr;

public:
    CString() = default;

    explicit CString(const char* s) {
        if (s) {
            const std::size_t n = std::strlen(s);
            data_ = new char[n + 1];
            std::memcpy(data_, s, n + 1);
        }
    }

    CString(const CString& o) {
        if (o.data_) {
            const std::size_t n = std::strlen(o.data_);
            data_ = new char[n + 1];
            std::memcpy(data_, o.data_, n + 1);
        }
    }

    CString& operator=(const CString& o) {
        if (this != &o) {
            CString tmp(o);  // 复用拷贝构造
            std::swap(data_, tmp.data_);
        }
        return *this;
    }

    ~CString() { delete[] data_; }

    // 注意：用户声明了拷贝/析构 → 移动被抑制 → std::move 走拷贝
    const char* c_str() const { return data_ ? data_ : ""; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [rule_of_three] ===\n";

    // -------------------------------------------------------------------------
    // §入门：三件套协同
    // -------------------------------------------------------------------------
    CString a{"hello"};
    CString b{a};
    assert(std::string(b.c_str()) == "hello");
    assert(a.c_str() != b.c_str());
    std::cout << "[intro] copy ctor deep: " << b.c_str() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：赋值 + 自赋值
    // -------------------------------------------------------------------------
    CString c{"world"};
    c = b;
    assert(std::string(c.c_str()) == "hello");
    c = c;
    assert(std::string(c.c_str()) == "hello");
    std::cout << "[advanced] copy assign ok\n";

    // -------------------------------------------------------------------------
    // §专家：缺一不可的后果（用注释+对照说明）
    // -------------------------------------------------------------------------
    // 若只写析构 delete[] 却用隐式浅拷贝：
    //   两个对象共享指针 → 双重 delete → UB
    // 若只写拷贝却忘析构：
    //   泄漏
    // 若只写赋值忘自赋值/顺序：
    //   自赋值释放后读野指针
    CString d = std::move(a);  // 实际拷贝（移动抑制）
    assert(std::string(d.c_str()) == "hello");
    assert(std::string(a.c_str()) == "hello");  // 源仍完整 → 非真正移动

    std::cout << "[expert] move suppressed → copy; src still '" << a.c_str() << "'\n";
    std::cout << "=== rule_of_three: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/rule_of_three", run>;

}  // namespace
