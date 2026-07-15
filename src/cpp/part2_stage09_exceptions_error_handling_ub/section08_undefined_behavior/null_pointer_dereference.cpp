// Topic    : 空指针解引用 UB —— 检查 / optional / 引用对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : null_pointer_dereference
// Topic id : part2/stage09/section08/null_pointer_dereference
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            UBSan null

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>

namespace {

[[maybe_unused]] int value_or_default(const int* p, int def) {
    if (p == nullptr) {
        return def;  // ✅ 先检查
    }
    return *p;
}

[[maybe_unused]] std::optional<int> as_optional(const int* p) {
    if (!p) {
        return std::nullopt;
    }
    return *p;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [null_pointer_dereference] 主干：用前检查 ===\n";
    {
        int x = 5;
        [[maybe_unused]] int* p = &x;
        assert(value_or_default(p, -1) == 5);
        assert(value_or_default(nullptr, -1) == -1);
        std::cout << "null-safe value_or_default ok\n";
    }

    std::cout << "=== 对抗：optional 表达可空 ===\n";
    {
        [[maybe_unused]] int x = 9;
        assert(as_optional(&x) == 9);
        assert(!as_optional(nullptr));
        std::cout << "optional from pointer\n";
    }

    std::cout << "=== 进阶：引用 = 非空契约；指针 = 可空 API ===\n";
    {
        // 引用在语言层面不能是“空”——必须绑定到对象
        int live = 11;
        [[maybe_unused]] const int& r = live;
        assert(r == 11);
        // API 设计：内部用引用（调用方已保证有效）；边界用 pointer/optional
        [[maybe_unused]] auto by_ref = [](const int& v) { return v * 2; };
        assert(by_ref(live) == 22);
        std::cout << "references encode non-null at the type level\n";
    }

    std::cout << "=== 专节：UB 形态（不触发）===\n";
    // 危险（勿运行）：
    //   int* p = nullptr; int v = *p;  // UB
    //   p->member;                     // UB
    //   delete p; 之后再 *p（use-after-free，见相邻 topic）
    // 引用必须绑定有效对象；不要“空引用”/重绑悬空。
    // 护栏：-fsanitize=undefined 可抓部分空解引用；ASan 抓 UAF。
    std::cout << "never dereference null; prefer references/optional\n";

    std::cout << "[null_pointer_dereference] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/null_pointer_dereference", run>;

}  // namespace
