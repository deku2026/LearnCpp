// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A8 type_info)
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : type_info
// Topic id : part6/a/section03/type_info
//
// 要点: std::type_info 描述类型; 由 typeid 产生, 不可值拷贝 (只能引用/指针持有);
//       name / before / hash_code / operator==; 多态类的 type_info 挂在 vtable 旁 (ABI);
//       name() 与 before() 均实现定义, 不可当作可移植稳定 ABI。
// 参考: [type.info] cppreference type_info

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace {

struct Widget {
    virtual ~Widget() = default;
};

struct Button : Widget {};
struct Label : Widget {};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== std::type_info ===\n";

    const std::type_info& ti_int = typeid(int);
    const std::type_info& ti_double = typeid(double);
    const std::type_info& ti_widget = typeid(Widget);
    const std::type_info& ti_button = typeid(Button);

    // --- 不可拷贝/赋值, 只能绑定引用或取址 ---
    // std::type_info copy = ti_int;  // ill-formed
    const std::type_info* ptr = &ti_int;
    assert(ptr == &typeid(int));
    assert(ptr->name() != nullptr);

    // --- name(): 实现定义 ---
    std::cout << "  int:    " << ti_int.name() << '\n';
    std::cout << "  double: " << ti_double.name() << '\n';
    std::cout << "  Widget: " << ti_widget.name() << '\n';
    std::cout << "  Button: " << ti_button.name() << '\n';

    // --- 相等 ---
    assert(ti_int == typeid(int));
    assert(ti_int != ti_double);
    assert(typeid(Button) != typeid(Label));

    Button b;
    Widget& w = b;
    assert(typeid(w) == ti_button);
    assert(typeid(w) != ti_widget);

    // --- hash_code ---
    assert(ti_button.hash_code() == typeid(Button).hash_code());
    std::cout << "  Button hash_code=" << ti_button.hash_code() << '\n';

    // --- before: 实现定义严格弱序 ---
    assert(!ti_int.before(ti_int));
    if (ti_int != ti_double) {
        assert(ti_int.before(ti_double) || ti_double.before(ti_int));
    }

    // --- std::type_index: 可拷贝, 适合容器键 ---
    std::unordered_map<std::type_index, std::string> registry;
    registry[std::type_index(typeid(Button))] = "ui.button";
    registry[std::type_index(typeid(Label))] = "ui.label";
    registry[std::type_index(typeid(int))] = "scalar.int";

    assert(registry[std::type_index(typeid(w))] == "ui.button");
    assert(registry[std::type_index(typeid(Label))] == "ui.label");
    assert(registry.size() == 3);

    // type_info 对象在程序整个运行期有效 — 可安全保存指针/引用
    const std::type_info* keep = &typeid(Button);
    assert(*keep == typeid(Button));

    // 与 dynamic_cast 关系: 运行期比较的就是各类型的 type_info 节点
    // (以及 ABI 中的继承图描述符; 见 Itanium RTTI 节)

    std::cout << "type_info: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section03/type_info", run>;

}  // namespace
