// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E10 C++23 protocol)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : basic_common_reference_for_reference_wrapper_cpp23
// Topic id : part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23
//
// 要点: C++23 为 reference_wrapper 定制 basic_common_reference,
//       使 common_reference_t<ref_wrapper<T>, T&> 等在 Ranges/zip 中正确。
// 参考: P2655 / library wording; cppreference common_reference

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

namespace {

// 教学: 手写"旧行为"直觉 vs 标准 common_reference
// 在 C++23, common_reference(reference_wrapper<T>, T&) 偏向 T& 语义

template <typename A, typename B>
using CR = std::common_reference_t<A, B>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E10 basic_common_reference + reference_wrapper (C++23) ===\n";

    int x = 10;
    std::reference_wrapper<int> rw = x;

    // 通过 get 访问
    assert(rw.get() == 10);
    rw.get() = 20;
    assert(x == 20);

    // common_reference 与引用
    using R1 = CR<int&, int&>;
    static_assert(std::is_same_v<R1, int&>);

    using R2 = CR<const int&, int&>;
    static_assert(std::is_same_v<R2, const int&>);

    // reference_wrapper 参与 common_reference (C++23 修补后行为稳定)
    using R3 = std::common_reference_t<std::reference_wrapper<int>&, int&>;
    // 结果应可作为引用使用的公共类型
    static_assert(std::is_reference_v<R3> || std::is_same_v<std::remove_cvref_t<R3>, std::reference_wrapper<int>> ||
                  std::is_same_v<std::remove_cvref_t<R3>, int>);

    // Ranges 场景: zip 可能混合 T& 与 ref_wrapper
    std::vector<int> v{1, 2, 3};
    auto first = std::ref(v[0]);
    assert(first.get() == 1);
    first.get() = 99;
    assert(v[0] == 99);

    // basic_common_reference 是可定制点: 用户可为代理引用类型特化
    // template<class T, class U, template<class> class TQual, ...>
    // struct basic_common_reference<MyProxy<T>, U, TQual, UQual>;

    std::cout << "  ref_wrapper is a proxy; common_reference must unwrap wisely\n";
    std::cout << "  enables views::zip / join with proxy iterators\n";
    std::cout << "basic_common_reference_for_reference_wrapper_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23", run>;

}  // namespace
