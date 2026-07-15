// Topic     : 用 deducing this 合并四份 ref-qualified 重载
// Doc       : 第2部分-阶段4 · 步骤 5.2
// cppreference: https://en.cppreference.com/cpp/language/member_functions
//
// 要点: template<class Self> auto&& data(this Self&& self) + forward
//       一次覆盖 const/非const × 左值/右值。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 传统四份
class Classic {
    std::string data_;

public:
    explicit Classic(std::string d) : data_(std::move(d)) {}

    std::string& data() & { return data_; }
    const std::string& data() const& { return data_; }
    std::string&& data() && { return std::move(data_); }
    const std::string&& data() const&& { return std::move(data_); }
};

// C++23 一份模板
class Widget {
    std::string data_;

public:
    explicit Widget(std::string d) : data_(std::move(d)) {}

    template <class Self>
    auto&& data(this Self&& self) {
        return std::forward<Self>(self).data_;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [collapse_ref_qualified_overloads] ===\n";

    // -------------------------------------------------------------------------
    // §入门：非 const 左值 → string&
    // -------------------------------------------------------------------------
    Widget w{"hello"};
    w.data() += "!";
    assert(w.data() == "hello!");
    static_assert(std::is_same_v<decltype(w.data()), std::string&>);
    std::cout << "[intro] lvalue non-const: " << w.data() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：const 左值 + 右值移动
    // -------------------------------------------------------------------------
    const Widget cw{"const"};
    static_assert(std::is_same_v<decltype(cw.data()), const std::string&>);
    assert(cw.data() == "const");

    std::string stolen = Widget{"tmp"}.data();  // 右值 → 可移动出
    assert(stolen == "tmp");
    std::cout << "[advanced] const lvalue + rvalue steal ok\n";

    // -------------------------------------------------------------------------
    // §专家：与经典四份对照；Self 推导直觉
    // -------------------------------------------------------------------------
    Classic c{"classic"};
    c.data() += "?";
    assert(c.data() == "classic?");
    std::string s2 = Classic{"x"}.data();
    assert(s2 == "x");

    // 调用非 const 左值：Self = Widget&
    // 调用 const 左值：  Self = const Widget&
    // 调用右值：        Self = Widget（或 Widget&& 依写法；forward 保持值类别）
    Widget w2{"z"};
    auto&& r = std::as_const(w2).data();
    static_assert(std::is_same_v<decltype(r), const std::string&>);

    std::cout << "[expert] classic parity; stolen=" << stolen << '\n';
    std::cout << "=== collapse_ref_qualified_overloads: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/collapse_ref_qualified_overloads", run>;

}  // namespace
