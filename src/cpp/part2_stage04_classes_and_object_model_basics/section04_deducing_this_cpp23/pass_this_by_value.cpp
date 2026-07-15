// Topic     : 按值传递 this（小对象）
// Doc       : 第2部分-阶段4 · 步骤 5.4
// cppreference: https://en.cppreference.com/cpp/language/member_functions
//
// 要点: this Point self 按值收对象，避免一层间接；适合小平凡类型；
//       大对象仍宜引用/转发引用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

struct Point {
    int x = 0;
    int y = 0;

    int sum(this Point self) { return self.x + self.y; }

    Point offset(this Point self, int dx, int dy) {
        self.x += dx;
        self.y += dy;
        return self;  // 返回修改后的副本，原对象不变
    }
};

struct Big {
    std::string payload;

    // 大对象：按值会昂贵；演示对比
    std::size_t size_by_value(this Big self) { return self.payload.size(); }

    std::size_t size_by_ref(this const Big& self) { return self.payload.size(); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pass_this_by_value] ===\n";

    // -------------------------------------------------------------------------
    // §入门：小对象按值
    // -------------------------------------------------------------------------
    Point p{3, 4};
    assert(p.sum() == 7);
    std::cout << "[intro] Point sum=" << p.sum() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：按值修改不影响原对象
    // -------------------------------------------------------------------------
    Point q = p.offset(1, 1);
    assert(p.x == 3 && p.y == 4);
    assert(q.x == 4 && q.y == 5);
    std::cout << "[advanced] offset copy (" << q.x << "," << q.y << ")\n";

    // -------------------------------------------------------------------------
    // §专家：何时按值 / 转发
    // -------------------------------------------------------------------------
    Big b{std::string(100, 'x')};
    assert(b.size_by_ref() == 100);
    assert(b.size_by_value() == 100);  // 可工作但拷贝重

    // 模板转发引用仍是通用默认：
    // template<class Self> auto f(this Self&& self);
    // 小对象特化可写 this Point self

    std::cout << "[expert] prefer by-value for tiny trivial types only\n";
    std::cout << "=== pass_this_by_value: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/pass_this_by_value", run>;

}  // namespace
