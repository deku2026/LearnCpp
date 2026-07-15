// Topic     : ref-qualified 成员函数
// Doc       : 第2部分-阶段4 · 步骤 4
// cppreference: https://en.cppreference.com/cpp/language/member_functions
//
// 要点: & 仅左值调用，&& 仅右值；用于「左值返回引用 / 右值移动资源」；
//       四份重载是 deducing this 要消灭的样板。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace {

class Widget {
    std::string data_;

public:
    explicit Widget(std::string d) : data_(std::move(d)) {}

    const std::string& data() const& { return data_; }
    std::string data() && { return std::move(data_); }

    // 禁止在右值上做某些原地修改（示例）
    void append(std::string_view s) & { data_.append(s); }
    // void append(std::string_view s) && = delete; // 可选
};

class Builder {
    std::string s_;

public:
    Builder& add(char c) & {
        s_.push_back(c);
        return *this;
    }

    // 右值限定：允许临时链式后移动结果
    std::string add(char c) && {
        s_.push_back(c);
        return std::move(s_);
    }

    const std::string& str() const& { return s_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ref_qualified_member_functions] ===\n";

    // -------------------------------------------------------------------------
    // §入门：左值引用 vs 右值移动
    // -------------------------------------------------------------------------
    Widget w{"hello"};
    const std::string& r = w.data();  // const&
    assert(r == "hello");
    std::string s = Widget{"tmp"}.data();  // &&
    assert(s == "tmp");
    std::cout << "[intro] lvalue ref + rvalue move: " << r << ' ' << s << '\n';

    // -------------------------------------------------------------------------
    // §进阶：& 限定禁止临时调用
    // -------------------------------------------------------------------------
    w.append("!");
    assert(w.data() == "hello!");
    // Widget{"x"}.append("y"); // ❌ 仅 & 重载

    Builder b;
    b.add('a').add('b');
    assert(b.str() == "ab");
    std::string out = Builder{}.add('z');
    assert(out == "z");
    std::cout << "[advanced] builder lvalue chain + rvalue finish\n";

    // -------------------------------------------------------------------------
    // §专家：四份样板（对照 deducing this）
    // -------------------------------------------------------------------------
    class Full {
        std::string d_;

    public:
        explicit Full(std::string d) : d_(std::move(d)) {}
        std::string& get() & { return d_; }
        const std::string& get() const& { return d_; }
        std::string&& get() && { return std::move(d_); }
        const std::string&& get() const&& { return std::move(d_); }
    };

    Full f{"f"};
    f.get() += "x";
    const Full cf{"c"};
    assert(cf.get() == "c");
    std::string m = Full{"m"}.get();
    assert(m == "m");

    std::cout << "[expert] four overloads work; prefer C++23 deducing this\n";
    std::cout << "=== ref_qualified_member_functions: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/ref_qualified_member_functions", run>;

}  // namespace
