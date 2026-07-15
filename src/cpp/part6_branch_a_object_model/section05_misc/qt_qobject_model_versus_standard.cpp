// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A10 Qt QObject 对照)
// Stage    : part6_branch_a_object_model
// Section  : section05_misc
// Item     : qt_qobject_model_versus_standard
// Topic id : part6/a/section05/qt_qobject_model_versus_standard
//
// 要点: Qt 在标准 C++ 对象模型之上自建元对象系统 (moc / QMetaObject);
//       qobject_cast 查 QMetaObject 链, dynamic_cast 查标准 RTTI;
//       本仓库不强制链 Qt — 用“迷你元对象”模拟对照, 讲清机制差异。
// 参考: https://doc.qt.io/qt-6/metaobjects.html

#include "learn/topic_registry.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <typeinfo>

namespace {

// ---------- 标准 C++ 侧: vtable + RTTI ----------
struct StdBase {
    virtual ~StdBase() = default;
    virtual std::string_view std_name() const { return "StdBase"; }
};

struct StdButton : StdBase {
    std::string_view std_name() const override { return "StdButton"; }
    void click() { clicked = true; }
    bool clicked = false;
};

struct StdLabel : StdBase {
    std::string_view std_name() const override { return "StdLabel"; }
};

// ---------- 迷你 “Qt-like” 元对象 (教学模拟, 非 Qt) ----------
struct MetaObject {
    const char* class_name;
    const MetaObject* super;
    // 真实 QMetaObject 还含信号/槽/属性/方法表等 — 此处从略
};

// 模拟 moc 为每个 Q_OBJECT 类生成静态元对象
struct MiniObject {
    virtual ~MiniObject() = default;
    virtual const MetaObject* metaObject() const = 0;

    // 模拟 qobject_cast: 沿 super 链比类名 (Qt 实际用更高效的内部 id)
    template <typename T>
    T* cast() {
        const MetaObject* m = metaObject();
        const MetaObject* target = &T::staticMetaObject;
        while (m) {
            if (m == target || std::strcmp(m->class_name, target->class_name) == 0) {
                return static_cast<T*>(this);
            }
            m = m->super;
        }
        return nullptr;
    }
};

struct MiniWidget : MiniObject {
    static constexpr MetaObject staticMetaObject{"MiniWidget", nullptr};
    const MetaObject* metaObject() const override { return &staticMetaObject; }
};

struct MiniButton : MiniWidget {
    static constexpr MetaObject staticMetaObject{"MiniButton", &MiniWidget::staticMetaObject};
    const MetaObject* metaObject() const override { return &staticMetaObject; }
    void animateClick() { pressed = true; }
    bool pressed = false;
};

struct MiniLabel : MiniWidget {
    static constexpr MetaObject staticMetaObject{"MiniLabel", &MiniWidget::staticMetaObject};
    const MetaObject* metaObject() const override { return &staticMetaObject; }
};

// 需要: constexpr MetaObject 地址稳定 — static constexpr 成员 OK
// MiniButton::staticMetaObject 等已是静态存储期

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== Qt QObject model vs standard C++ (simulated) ===\n";

    // --- 标准 dynamic_cast ---
    StdButton btn;
    StdBase* sp = &btn;
    if (auto* b = dynamic_cast<StdButton*>(sp)) {
        b->click();
        assert(b->clicked);
    } else {
        assert(false);
    }
    assert(dynamic_cast<StdLabel*>(sp) == nullptr);
    assert(typeid(*sp) == typeid(StdButton));
    std::cout << "  standard dynamic_cast + typeid: " << typeid(*sp).name() << '\n';

    // --- 模拟 qobject_cast (元对象链) ---
    MiniButton mb;
    MiniObject* op = &mb;
    MiniButton* qb = op->cast<MiniButton>();
    assert(qb != nullptr);
    qb->animateClick();
    assert(mb.pressed);

    [[maybe_unused]] MiniLabel* fail = op->cast<MiniLabel>();
    assert(fail == nullptr);

    [[maybe_unused]] MiniWidget* as_widget = op->cast<MiniWidget>();
    assert(as_widget != nullptr);  // 向上沿 super 链成功

    std::cout << "  mini meta class: " << op->metaObject()->class_name << '\n';
    std::cout << "  super: " << op->metaObject()->super->class_name << '\n';

    // --- 对照表 (文档 A10) ---
    std::cout << "\n  | concern | standard dynamic_cast      | Qt qobject_cast (real)   |\n"
              << "  |----------|-----------------------------|--------------------------|\n"
              << "  | engine   | RTTI / type_info / vtable   | QMetaObject chain (moc)  |\n"
              << "  | needs    | polymorphic type + RTTI     | Q_OBJECT + moc output    |\n"
              << "  | domain   | any polymorphic C++ class   | QObject hierarchy only   |\n"
              << "  | cross-DLL| RTTI visibility sensitive   | often more stable in Qt  |\n";

    // 关键认知:
    // 1) Qt 信号槽/属性/QML 反射不来自标准 C++ RTTI, 而来自 moc 生成代码。
    // 2) 标准对象模型 (本支线 A1–A9) 仍在: QObject 一样有 vptr/继承布局。
    // 3) 选工具: QObject 树内用 qobject_cast; 非 Qt 多态用 dynamic_cast/虚函数。
    // 4) 本文件不 #include Qt — 避免强依赖; 有 Qt 环境时对照 moc_*.cpp 更直观。

    // 双轨并存示意: 同一概念两种查询
    [[maybe_unused]] StdBase* std_p = &btn;
    [[maybe_unused]] MiniObject* mini_p = &mb;
    assert(dynamic_cast<StdButton*>(std_p) != nullptr);
    assert(mini_p->cast<MiniButton>() != nullptr);

    std::cout << "qt_qobject_model_versus_standard: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section05/qt_qobject_model_versus_standard", run>;

}  // namespace
