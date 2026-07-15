// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : small_buffer_optimization_in_erasure
// Topic id : part6/e/section06/small_buffer_optimization_in_erasure
//
// Covers: SBO sketch inside type erasure

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <new>
#include <utility>

namespace {

class SmallErased {
    static constexpr std::size_t SBO = 16;
    struct Concept {
        virtual ~Concept() = default;
        virtual int get() const = 0;
        virtual Concept* move_to(void* buf) = 0;
    };
    template <class T>
    struct Model : Concept {
        T v;
        explicit Model(T x) : v(std::move(x)) {}
        int get() const override { return v; }
        Concept* move_to(void* buf) override { return new (buf) Model(std::move(v)); }
    };
    alignas(std::max_align_t) unsigned char buf_[SBO];
    Concept* ptr_ = nullptr;
    bool local_ = false;

public:
    template <class T>
    explicit SmallErased(T x) {
        if (sizeof(Model<T>) <= SBO) {
            ptr_ = new (buf_) Model<T>(std::move(x));
            local_ = true;
        } else {
            ptr_ = new Model<T>(std::move(x));
            local_ = false;
        }
    }
    SmallErased(SmallErased&& o) noexcept {
        if (o.local_) {
            ptr_ = o.ptr_->move_to(buf_);
            local_ = true;
            o.ptr_->~Concept();
            o.ptr_ = nullptr;
        } else {
            ptr_ = o.ptr_;
            local_ = false;
            o.ptr_ = nullptr;
        }
    }
    ~SmallErased() {
        if (!ptr_) return;
        if (local_)
            ptr_->~Concept();
        else
            delete ptr_;
    }
    int get() const { return ptr_->get(); }
};

void demo_basics() {
    SmallErased e{5};
    assert(e.get() == 5);
}

void demo_intermediate() {
    SmallErased a{7};
    SmallErased b{std::move(a)};
    assert(b.get() == 7);
}

void demo_expert() {
    SmallErased e{11};
    assert(e.get() == 11);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/small_buffer_optimization_in_erasure", run>;

}  // namespace
