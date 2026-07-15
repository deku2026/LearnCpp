// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 qualified)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : qualified_lookup
// Topic id : part6/d/section01/qualified_lookup
//
// 要点: A::b / ::b 只在指定作用域里找。
// 参考: [basic.lookup.qual]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

namespace lib {
struct Widget {
    [[maybe_unused]] static constexpr int id = 7;
    int value = 0;
    int get() const { return value; }
};
[[maybe_unused]] int tag = 42;
namespace nested {
[[maybe_unused]] int deep = 9;
}
}  // namespace lib

struct Outer {
    struct Inner {
        [[maybe_unused]] static constexpr int k = 3;
    };
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 qualified lookup ===\n";

    assert(lib::tag == 42);
    assert(lib::Widget::id == 7);
    assert(lib::nested::deep == 9);

    lib::Widget w;
    w.value = 3;
    assert(w.get() == 3);
    assert(::lib::tag == 42);

    assert(Outer::Inner::k == 3);

    std::vector<int> v{1, 2};
    assert(v.size() == 2);

    // 限定查找不做 ADL；也跳过内层同名隐藏问题（直接点名）
    std::cout << "  qualified: look only in named scope; no outer walk\n";
    std::cout << "qualified_lookup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/qualified_lookup", run>;

}  // namespace
