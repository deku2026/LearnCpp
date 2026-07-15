// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D4 template 消歧)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : template_disambiguation
// Topic id : part6/d/section02/template_disambiguation
//
// 要点: 依赖成员模板调用需 .template / ->template；否则 < 被当小于号。
// 参考: [temp.names]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

struct Processor {
    template <typename U>
    U process(U x) const {
        return static_cast<U>(x * 2);
    }
};

template <typename T>
struct Container {
    T obj;
    template <typename U>
    U call_process() {
        // obj.process<U>() 错误解析为比较
        return obj.template process<U>(static_cast<U>(3));
    }
};

template <typename T>
struct Outer {
    template <typename U>
    struct Inner {
        static constexpr int value = sizeof(U) + sizeof(T);
    };
};

template <typename T>
int nested_size() {
    return Outer<T>::template Inner<int>::value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D4 template disambiguation ===\n";

    [[maybe_unused]] Container<Processor> c{};
    assert(c.call_process<int>() == 6);
    assert(c.call_process<double>() == 6.0);

    assert(nested_size<char>() == static_cast<int>(sizeof(int) + sizeof(char)));

    std::cout << "  use .template mem<Args>() after dependent object\n";
    std::cout << "template_disambiguation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section02/template_disambiguation", run>;

}  // namespace
