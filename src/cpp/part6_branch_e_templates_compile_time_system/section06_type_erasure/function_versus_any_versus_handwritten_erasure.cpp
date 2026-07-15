// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : function_versus_any_versus_handwritten_erasure
// Topic id : part6/e/section06/function_versus_any_versus_handwritten_erasure
//
// Covers: std::function vs any vs manual type erasure

#include "learn/topic_registry.hpp"

#include <any>
#include <functional>
#include <memory>

namespace {

struct ErasedInt {
    struct Concept {
        virtual ~Concept() = default;
        virtual int get() const = 0;
    };
    template <class T>
    struct Model : Concept {
        T v;
        explicit Model(T x) : v(std::move(x)) {}
        int get() const override { return static_cast<int>(v); }
    };
    std::unique_ptr<Concept> self;
    template <class T>
    explicit ErasedInt(T x) : self(std::make_unique<Model<T>>(std::move(x))) {}
    int get() const { return self->get(); }
};

void demo_basics() {
    std::function<int(int)> f = [](int x) { return x + 1; };
    LEARN_CHECK(f(2) == 3);
}

void demo_intermediate() {
    std::any a = 5;
    LEARN_CHECK(std::any_cast<int>(a) == 5);
}

void demo_expert() {
    ErasedInt e{9};
    LEARN_CHECK(e.get() == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section06/function_versus_any_versus_handwritten_erasure", run>;

}  // namespace
