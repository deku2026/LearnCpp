// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B7 gsl::not_null)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : not_null_annotation_gsl
// Topic id : part6/b/section04/not_null_annotation_gsl
//
// 要点: not_null 表示非空借用；调用方不必查 null。本地薄包装演示语义。
// 参考: C++ Core Guidelines F.23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {

template <class Ptr>
class not_null {
    Ptr p_;

public:
    explicit not_null(Ptr p) : p_(p) {
        if (!p_) throw std::invalid_argument("not_null: nullptr");
    }
    not_null& operator=(Ptr p) {
        if (!p) throw std::invalid_argument("not_null: nullptr");
        p_ = p;
        return *this;
    }
    Ptr get() const { return p_; }
    auto& operator*() const { return *p_; }
    Ptr operator->() const { return p_; }
    operator Ptr() const { return p_; }
};

struct Widget {
    int id;
};

// 借用且非空：不拥有、不 delete
void process(not_null<Widget*> w) {
    // 无需 if (!w)
    w->id += 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B7 not_null annotation (GSL-style) ===\n";

    Widget w{10};
    process(not_null<Widget*>{&w});
    assert(w.id == 11);

    not_null<Widget*> nn{&w};
    assert(nn->id == 11);
    // Widget 无 operator==：比较指针身份与字段
    assert(nn.get() == &w);
    assert((*nn).id == w.id);

    bool threw = false;
    try {
        Widget* nullp = nullptr;
        not_null<Widget*> bad{nullp};
        (void)bad;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // 引用本身已非空：优先用 T& 当非空借用
    auto by_ref = [](Widget& r) { r.id += 1; };
    by_ref(w);
    assert(w.id == 12);

    std::cout << "  not_null: non-owning non-null borrow; prefer T& when possible\n";
    std::cout << "not_null_annotation_gsl: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/not_null_annotation_gsl", run>;

}  // namespace
