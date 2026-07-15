// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B8 constructs_from_temporary / B12)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_constructs_from_temporary_cpp23
// Topic id : part6/b/section05/reference_constructs_from_temporary_cpp23
//
// 要点: 编译期检测「用 U 初始化 T 引用是否会绑到临时」。
// 验收: 用 trait + static_assert 拦会绑临时的接口。
// 参考: https://en.cppreference.com/w/cpp/types/reference_constructs_from_temporary  P2255

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class Ref, class Arg>
constexpr bool dangerous_bind = std::reference_constructs_from_temporary_v<Ref, Arg>;

// 进阶: 存引用的「槽」——拒绝会物化临时的实参
template <class T>
struct RefSlot {
    const T* ptr = nullptr;

    template <class Arg>
    void bind(Arg&& arg) {
        static_assert(!std::reference_constructs_from_temporary_v<const T&, Arg>,
                      "RefSlot: argument would bind reference to a temporary");
        ptr = std::addressof(static_cast<const T&>(arg));
    }

    const T& get() const {
        assert(ptr != nullptr);
        return *ptr;
    }
};

template <class T, class Arg>
void store_ref_checked(const T& /*slot*/, Arg&& /*arg*/) {
    static_assert(!std::reference_constructs_from_temporary_v<const T&, Arg>,
                  "argument would bind reference to a temporary");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B8 reference_constructs_from_temporary (C++23) ===\n";

#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    // --- 入门: trait 真值表 ---
    static_assert(std::reference_constructs_from_temporary_v<const std::string&, std::string>);
    static_assert(!std::reference_constructs_from_temporary_v<const std::string&, const std::string&>);
    static_assert(!std::reference_constructs_from_temporary_v<const int&, int&>);
    static_assert(std::reference_constructs_from_temporary_v<const int&, int>);  // prvalue int → 临时

    assert((dangerous_bind<const std::string&, std::string>));
    assert((!dangerous_bind<const std::string&, const std::string&>));
    assert((dangerous_bind<const int&, int>));
    assert((!dangerous_bind<const int&, const int&>));

    // --- 进阶: 安全 API 门禁 ---
    std::string owned = "ok";
    store_ref_checked(owned, owned);  // 安全：已有对象
    // store_ref_checked(owned, std::string("tmp")); // 编译失败（static_assert）

    RefSlot<std::string> slot;
    slot.bind(owned);
    assert(slot.get() == "ok");
    // slot.bind(std::string("nope")); // 编译失败

    // --- 专家: constructs vs converts ---
    // constructs: 初始化引用时是否从临时物化绑定
    // converts:   更宽，含隐式转换路径产生临时（见 reference_converts_from_temporary）
    // 注意: const string& ← string&& 绑到已有 xvalue，通常不视为「从临时构造」
    static_assert(!std::reference_constructs_from_temporary_v<const std::string&, std::string&>);
    static_assert(std::reference_constructs_from_temporary_v<const std::string&, std::string>);

    std::cout << "  true  => initializing ref would materialize temporary\n";
    std::cout << "  false => binds to existing object\n";
    std::cout << "  use in APIs that store references across statements\n";
#else
    std::cout << "  trait not available on this library; skipped asserts\n";
    std::string owned = "ok";
    store_ref_checked(owned, owned);
#endif

    std::cout << "reference_constructs_from_temporary_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/reference_constructs_from_temporary_cpp23", run>;

}  // namespace
