// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section05_misc
// Item     : qt_qobject_model_versus_standard
// Topic id : part6/a/section05/qt_qobject_model_versus_standard
//
// Covers: contrast Qt QObject identity/meta-object idea with standard C++ object model (no Qt dep)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

// Minimal stand-in illustrating "meta-object" style identity vs plain C++ layout.
struct MetaObject {
    std::string_view class_name;
    const MetaObject* super = nullptr;
};

struct ObjectBase {
    virtual ~ObjectBase() = default;
    virtual const MetaObject* meta() const = 0;
    virtual std::string object_name() const { return name_; }
    void set_object_name(std::string n) { name_ = std::move(n); }

private:
    std::string name_;
};

struct Widget : ObjectBase {
    static constexpr MetaObject meta_data{"Widget", nullptr};
    const MetaObject* meta() const override { return &meta_data; }
    int width = 0;
};

void demo_basics() {
    Widget w;
    w.set_object_name("main");
    assert(w.object_name() == "main");
    assert(w.meta()->class_name == "Widget");
}

void demo_intermediate() {
    // Standard C++: dynamic type via vtable/RTTI; no built-in property system.
    Widget w;
    ObjectBase* p = &w;
    assert(p->meta()->class_name == "Widget");
    // Qt adds signals/slots, parent ownership tree, moc-generated meta calls —
    // those are framework conventions, not ISO object layout rules.
    assert(sizeof(Widget) >= sizeof(void*));
}

void demo_expert() {
    // QObject is non-copyable and identity-based; value semantics differ from POD structs.
    Widget a;
    Widget b;
    a.set_object_name("a");
    b.set_object_name("b");
    assert(a.object_name() != b.object_name());
    // Standard layout/trivial traits usually fail for polymorphic framework bases.
    assert(a.meta() == &Widget::meta_data);
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section05/qt_qobject_model_versus_standard", run>;

}  // namespace
