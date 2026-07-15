// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E1 实例化模型)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section01_instantiation_model
// Item     : two_phase_lookup_recap
// Topic id : part6/e/section01/two_phase_lookup_recap
//
// 要点: 模板名查找分两阶段 ——
//   1) 定义点: 非依赖名立即查找/绑定;
//   2) 实例化点: 依赖名再次查找, 并可触发 ADL。
// 参考: [temp.res] [temp.dep] cppreference "Dependent names" / two-phase lookup

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace lib {

// 定义在模板定义之前 → 第一阶段就能看到
inline std::string non_dependent_tag() {
    return "lib::non_dependent_tag";
}

struct Widget {
    int id = 1;
};

// ADL 候选: 参数类型 Widget 关联到 lib
inline std::string describe(const Widget& w) {
    return "lib::describe(Widget id=" + std::to_string(w.id) + ")";
}

}  // namespace lib

namespace {

// 非依赖名: 在模板定义时解析, 不依赖 T
template <typename T>
std::string phase1_only() {
    // non_dependent_tag 不是依赖名 → 定义点查找
    return lib::non_dependent_tag();
}

// 依赖名: f(t) 的查找推迟到实例化, 可走 ADL
template <typename T>
std::string call_describe(const T& t) {
    using std::to_string;  // 仅为对照, 不参与 describe
    // describe 依赖 T → 第二阶段 + ADL
    return describe(t);
}

// typename / template 消歧: 依赖嵌套名
template <typename Cont>
typename Cont::value_type first_or_default(const Cont& c) {
    if (c.empty()) {
        return typename Cont::value_type{};
    }
    return *c.begin();
}

// 依赖模板成员需 template 关键字
template <typename T>
struct Holder {
    template <typename U>
    static U cast_from(T v) {
        return static_cast<U>(v);
    }
};

template <typename T>
int cast_via_holder(T v) {
    // 依赖名成员模板: 需要 ::template 消歧
    return Holder<T>::template cast_from<int>(v);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E1 two-phase lookup recap ===\n";

    // --- 入门: 非依赖名在定义点绑定 ---
    assert(phase1_only<int>() == "lib::non_dependent_tag");
    std::cout << "  phase1 non-dependent: " << phase1_only<double>() << '\n';

    // --- 进阶: 依赖名 + ADL ---
    lib::Widget w{42};
    const auto s = call_describe(w);
    assert(s.find("id=42") != std::string::npos);
    std::cout << "  phase2 + ADL: " << s << '\n';

    // --- 专家: typename / .template 消歧 ---
    std::vector<int> v{7, 8, 9};
    assert(first_or_default(v) == 7);
    assert(first_or_default(std::vector<int>{}) == 0);
    assert(cast_via_holder(3.9) == 3);
    std::cout << "  typename/template disambiguation OK\n";

    std::cout << "  rule: dependent names wait until instantiation;"
                 " non-dependent bind at definition.\n";
    std::cout << "two_phase_lookup_recap: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section01/two_phase_lookup_recap", run>;

}  // namespace
