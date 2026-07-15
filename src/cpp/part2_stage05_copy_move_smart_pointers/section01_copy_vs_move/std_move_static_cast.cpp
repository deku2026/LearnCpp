// Topic    : std::move 只是 static_cast 到右值（本身不移动任何东西）
// Doc      : 第2部分-阶段5 · 步骤 2
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : std_move_static_cast
// Topic id : part2/stage05/section01/std_move_static_cast
// Refs     : https://en.cppreference.com/w/cpp/utility/move
//            Effective Modern C++ Item 23；ISO [utility.move]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// std::move 的实现本质（简化）：无条件 cast 成右值引用
template <class T>
constexpr std::remove_reference_t<T>&& my_move(T&& x) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(x);
}

struct Tracker {
    std::string tag;
    int copies = 0;
    int moves = 0;

    explicit Tracker(std::string t) : tag(std::move(t)) {}

    Tracker(const Tracker& o) : tag(o.tag), copies(o.copies + 1), moves(o.moves) {
        std::cout << "  copy-ctor of [" << tag << "]\n";
    }

    Tracker(Tracker&& o) noexcept : tag(std::move(o.tag)), copies(o.copies), moves(o.moves + 1) {
        o.tag = "moved-from";
        std::cout << "  move-ctor -> [" << tag << "]\n";
    }

    Tracker& operator=(const Tracker& o) {
        tag = o.tag;
        copies = o.copies + 1;
        moves = o.moves;
        std::cout << "  copy-assign [" << tag << "]\n";
        return *this;
    }

    Tracker& operator=(Tracker&& o) noexcept {
        tag = std::move(o.tag);
        copies = o.copies;
        moves = o.moves + 1;
        o.tag = "moved-from";
        std::cout << "  move-assign -> [" << tag << "]\n";
        return *this;
    }
};

// 只有拷贝、没有移动：move 会退化为拷贝
struct CopyOnly {
    std::string s;
    explicit CopyOnly(std::string v) : s(std::move(v)) {}
    CopyOnly(const CopyOnly& o) : s(o.s) { std::cout << "  CopyOnly copy\n"; }
    CopyOnly& operator=(const CopyOnly& o) {
        s = o.s;
        std::cout << "  CopyOnly copy-assign\n";
        return *this;
    }
    // 故意不声明移动 → 编译器因用户声明了拷贝而不生成移动
};

void take_lvalue(std::string&) {
    std::cout << "  overload: string&\n";
}
void take_rvalue(std::string&&) {
    std::cout << "  overload: string&&\n";
}

// 具名右值引用参数：表达式仍是 lvalue
void sink_named_rref(std::string&& r) {
    // take_rvalue(r);           // 错误：r 是 lvalue
    take_rvalue(std::move(r));  // 必须再 move
    (void)r;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_move_static_cast] 入门：move = cast ===\n";

    std::string a = "hello";
    std::string b = my_move(a);  // 等价 std::move(a)
    assert(b == "hello");
    // a 通常为空，但不依赖具体实现值
    std::cout << "my_move: b=[" << b << "] a.empty()=" << a.empty() << '\n';

    // 类型层面：std::move 结果是 xvalue / 右值引用
    int x = 1;
    using MoveResult = decltype(std::move(x));
    static_assert(std::is_same_v<MoveResult, int&&>);
    static_assert(std::is_rvalue_reference_v<MoveResult>);

    std::cout << "=== 进阶：const move 退化；CopyOnly 退化 ===\n";
    {
        const std::string c = "world";
        std::string d = std::move(c);  // const 右值 → 只能绑拷贝构造
        assert(d == "world");
        assert(c == "world");  // const 源未被修改
        std::cout << "const move degenerates to copy: c still=[" << c << "]\n";

        CopyOnly co1{"payload"};
        CopyOnly co2 = std::move(co1);  // 无移动构造 → 拷贝
        assert(co2.s == "payload");
        assert(co1.s == "payload");  // 源仍完整（拷贝）
        std::cout << "CopyOnly + move still copied: co1=[" << co1.s << "]\n";
    }

    std::cout << "=== 进阶：具名 && 是 lvalue；Tracker 计数 ===\n";
    {
        std::string s = "x";
        sink_named_rref(std::move(s));

        Tracker t1{"T"};
        Tracker t2 = std::move(t1);
        assert(t2.moves >= 1);
        assert(t1.tag == "moved-from");
        std::cout << "Tracker moves=" << t2.moves << " copies=" << t2.copies << '\n';
    }

    std::cout << "=== 专家：何时该 move / 误用 ===\n";
    // · move 只是信号：「我不再需要这个值」
    // · 真移动发生在选中的移动构造/赋值函数体
    // · 误用：move 后继续读值（除非无前置条件 API）；move const；对还要用的对象 move
    // · 对 int 等标量 move 无收益（就是拷贝），但无害
    int n = 42;
    int m = std::move(n);
    assert(m == 42);
    // n 仍可读（标量移动=拷贝），但风格上 move 后不应再依赖
    (void)n;

    std::cout << "=== std_move_static_cast: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/std_move_static_cast", run>;

}  // namespace
