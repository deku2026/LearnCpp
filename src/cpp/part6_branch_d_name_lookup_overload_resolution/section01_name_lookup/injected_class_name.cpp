// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 注入类名)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : injected_class_name
// Topic id : part6/d/section01/injected_class_name
//
// 要点: 类内名字 Foo 即当前类；模板中指当前特化。
// 参考: [class.qual] injected-class-name

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

struct Foo {
    using Self = Foo;  // 注入类名
    int v = 0;
    Foo* self() { return this; }
    Self& ref() { return *this; }
};

template <typename T>
struct Box {
    using type = Box;  // 当前特化
    T value{};
    Box* identity() { return this; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 injected class name ===\n";

    Foo f;
    f.v = 5;
    assert(f.self()->v == 5);
    assert(f.ref().v == 5);
    static_assert(std::is_same_v<Foo::Self, Foo>);

    Box<int> b;
    b.value = 9;
    assert(b.identity()->value == 9);
    static_assert(std::is_same_v<Box<int>::type, Box<int>>);

    std::cout << "injected_class_name: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/injected_class_name", run>;

}  // namespace
