// Topic    : const T& / T&& 绑定临时量可延长生命周期
// Doc      : 第2部分-阶段5 · 步骤 11.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : const_ref_extends_temporary
// Topic id : part2/stage05/section05/const_ref_extends_temporary
// Refs     : https://en.cppreference.com/w/cpp/language/reference_initialization
//            ISO [class.temporary]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

int g_alive = 0;

struct Guard {
    std::string tag;
    explicit Guard(std::string t) : tag(std::move(t)) {
        ++g_alive;
        std::cout << "  +Guard " << tag << '\n';
    }
    ~Guard() {
        --g_alive;
        std::cout << "  -Guard " << tag << '\n';
    }
    const std::string& name() const { return tag; }
};

Guard make_guard(std::string t) {
    return Guard{std::move(t)};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [const_ref_extends_temporary] 入门：直接绑定延长 ===\n";

    g_alive = 0;
    {
        const Guard& r = make_guard("extend");
        // 临时 Guard 生命周期延长到 r 的作用域结束
        assert(g_alive == 1);
        assert(r.tag == "extend");
        std::cout << "inside scope: alive=" << g_alive << " tag=" << r.tag << '\n';
    }
    assert(g_alive == 0);

    std::cout << "=== 进阶：右值引用同样可延长 ===\n";
    {
        Guard&& rr = Guard{"rv"};
        assert(g_alive == 1);
        assert(rr.tag == "rv");
    }
    assert(g_alive == 0);

    std::cout << "=== 进阶：延长不穿透到成员 / 下标 ===\n";
    {
        // 危险模式（不要解引用使用）：
        // const char& c = make_guard("x").tag[0];
        // 临时 Guard 在本完整表达式末尾销毁，c 悬垂。
        // 正确：先具名延长整个对象
        const Guard& g = make_guard("safe");
        const char& c = g.tag[0];
        assert(c == 's');
        std::cout << "bind member via named extended object: c=" << c << '\n';
    }

    {
        // string 临时 + 下标：完整表达式结束后 string 销毁
        // const char& bad = std::string("ab")[0]; // 悬垂
        const std::string& s = std::string("ab");  // 延长整个 string
        const char& ch = s[0];
        assert(ch == 'a');
        std::cout << "extend whole string then index: " << ch << '\n';
    }

    std::cout << "=== 专家：哪些引用能延长 ===\n";
    // · const T& 与 T&& 绑定 prvalue/xvalue 临时量时可延长
    // · 非 const T& 不能绑临时量（编译错误）
    // · 函数返回的引用不会「带回」对临时量的延长
    // · 多级：只有直接绑定的那个临时量被延长

    // int& bad = 1;  // 非法
    [[maybe_unused]] const int& ok = 1;  // 延长 int 临时量
    assert(ok == 1);

    std::cout << "=== const_ref_extends_temporary: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/const_ref_extends_temporary", run>;

}  // namespace
