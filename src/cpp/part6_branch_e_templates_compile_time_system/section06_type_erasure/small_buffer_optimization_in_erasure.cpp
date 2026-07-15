// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E6 type erasure)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : small_buffer_optimization_in_erasure
// Topic id : part6/e/section06/small_buffer_optimization_in_erasure
//
// 要点: SBO = 小对象存在本地缓冲, 大对象堆分配; function/any 常见实现。
// 参考: LLVM Function; MSVC std::function SBO

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <iostream>
#include <new>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 简化 SBO 类型擦除: 调用 int()
class SmallFunction {
    static constexpr std::size_t BufSize = 32;
    static constexpr std::size_t BufAlign = alignof(std::max_align_t);

    struct VTable {
        int (*invoke)(void*);
        void (*destroy)(void*);
        void (*clone)(void* dst, const void* src);
        bool local;
    };

    alignas(BufAlign) unsigned char buf_[BufSize];
    void* ptr_ = nullptr;
    const VTable* vt_ = nullptr;

    template <typename F>
    static const VTable* vtable_for() {
        static const VTable vt{
            // invoke
            [](void* p) -> int { return (*static_cast<F*>(p))(); },
            // destroy
            [](void* p) { static_cast<F*>(p)->~F(); },
            // clone
            [](void* dst, const void* src) { ::new (dst) F(*static_cast<const F*>(src)); },
            // local?
            sizeof(F) <= BufSize && alignof(F) <= BufAlign && std::is_nothrow_move_constructible_v<F>,
        };
        return &vt;
    }

    void* storage_for(std::size_t size, std::size_t /*align*/, bool local) {
        if (local) {
            return buf_;
        }
        // 简化: 超大走 new（与 reset 里 delete 配对；避免 align_val_t 不匹配）
        return ::operator new(size);
    }

public:
    SmallFunction() = default;

    template <typename F>
    SmallFunction(F f) {
        using T = std::decay_t<F>;
        vt_ = vtable_for<T>();
        const bool local = vt_->local;
        ptr_ = storage_for(sizeof(T), alignof(T), local);
        ::new (ptr_) T(std::move(f));
        if (!local) {
            // heap pointer already in ptr_
        } else {
            ptr_ = buf_;
        }
    }

    SmallFunction(const SmallFunction& o) {
        if (!o.vt_) {
            return;
        }
        vt_ = o.vt_;
        const bool local = vt_->local;
        if (local) {
            ptr_ = buf_;
            vt_->clone(ptr_, o.ptr_);
        } else {
            // 重新分配并 clone —— 此处简化: 只支持 local 的拷贝教学
            // 对 heap 对象用 clone 到新缓冲
            ptr_ = ::operator new(BufSize);  // 简化
            vt_->clone(ptr_, o.ptr_);
        }
    }

    SmallFunction& operator=(SmallFunction o) {
        swap(o);
        return *this;
    }

    SmallFunction(SmallFunction&& o) noexcept { move_from(std::move(o)); }

    ~SmallFunction() { reset(); }

    void swap(SmallFunction& o) noexcept {
        SmallFunction tmp{std::move(o)};
        o.move_from(std::move(*this));
        move_from(std::move(tmp));
    }

    explicit operator bool() const { return vt_ != nullptr; }

    int operator()() const {
        assert(vt_);
        return vt_->invoke(ptr_);
    }

private:
    void reset() {
        if (vt_) {
            vt_->destroy(ptr_);
            if (!vt_->local) {
                ::operator delete(ptr_);
            }
            vt_ = nullptr;
            ptr_ = nullptr;
        }
    }

    void move_from(SmallFunction&& o) noexcept {
        reset();
        vt_ = o.vt_;
        if (!vt_) {
            return;
        }
        if (vt_->local) {
            // 重新定位到自己的 buf: 需要 move-construct
            // 简化教学: 要求 F 可平凡 relocate 不现实;
            // 用 clone + destroy 模拟 move
            ptr_ = buf_;
            vt_->clone(ptr_, o.ptr_);
            o.reset();
        } else {
            ptr_ = o.ptr_;
            o.ptr_ = nullptr;
            o.vt_ = nullptr;
        }
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E6 SBO in type erasure ===\n";

    // 小 lambda → 本地缓冲
    SmallFunction f = [] { return 42; };
    assert(f());
    assert(f() == 42);

    int capture = 7;
    SmallFunction g = [capture] { return capture * 3; };
    assert(g() == 21);

    // 大捕获 → 堆 (超过 BufSize)
    struct Big {
        char data[64]{};
        int operator()() const { return data[0] + 1; }
    };
    Big big{};
    big.data[0] = 8;
    SmallFunction h = big;
    assert(h() == 9);

    std::cout << "  small callables avoid heap; large fall back to new\n";
    std::cout << "  real std::function SBO size is implementation-defined\n";
    std::cout << "small_buffer_optimization_in_erasure: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/small_buffer_optimization_in_erasure", run>;

}  // namespace
