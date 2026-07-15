// Topic     : C++23 简化的隐式移动（P2266）
// Doc       : 第2部分-阶段2 · 步骤 5.6
// cppreference: https://en.cppreference.com/cpp/language/return
//               https://en.cppreference.com/cpp/language/copy_elision
//               https://en.cppreference.com/cpp/language/value_category
// 提案     : P2266R3 Simpler implicit move
//
// 要点: return 局部自动对象时，move-eligible 的 id-expression 按 xvalue 处理；
//       C++23 去掉若干“先当 lvalue 尝试”的旧两步规则，隐式移动更可预期。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

// 追踪拷贝/移动次数
struct Tracer {
    int id{0};
    inline static int copies = 0;
    inline static int moves = 0;

    explicit Tracer(int i) : id{i} {}
    Tracer(const Tracer& o) : id{o.id} { ++copies; }
    Tracer(Tracer&& o) noexcept : id{o.id} {
        ++moves;
        o.id = -1;
    }
    Tracer& operator=(const Tracer&) = delete;
    Tracer& operator=(Tracer&&) = delete;
};

void reset_counts() {
    Tracer::copies = 0;
    Tracer::moves = 0;
}

Tracer returns_by_value() {
    Tracer local{1};
    return local;  // 隐式移动（C++11 起在多数情况下）；C++23 规则更简
}

// 返回右值引用：C++23 P2266 让 move-eligible 的局部在返回时始终当 xvalue
Tracer&& returns_rref_to_local_support() {
    // 注意：返回局部的引用本身是悬空——此处只演示“值类别/重载选择”教学时
    // 不应在生产代码返回局部引用。下面用静态存储避免 UB 做可运行演示。
    static Tracer storage{2};
    storage.id = 2;
    return std::move(storage);
}

std::string by_value_string() {
    std::string s = "implicit";
    return s;  // 隐式移动进返回值（或 NRVO）
}

// 抛出时也有类似隐式移动规则（throw local;）
Tracer make_for_throw() {
    Tracer t{3};
    return t;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [simplified_implicit_move_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // 入门：return 局部 → 优先移动
    // -------------------------------------------------------------------------
    reset_counts();
    [[maybe_unused]] Tracer a = returns_by_value();
    assert(a.id == 1);
    // 在无 NRVO 时至少应看到 moves>=1 且 copies==0；有 NRVO 则 copies=moves=0
    assert(Tracer::copies == 0);
    std::cout << "[intro] return local: copies=" << Tracer::copies << " moves=" << Tracer::moves
              << " (0/0 means NRVO)\n";

    std::string s = by_value_string();
    assert(s == "implicit");
    std::cout << "[intro] string return uses move or NRVO, not deep copy of local\n";

    // -------------------------------------------------------------------------
    // 进阶：C++23 简化了什么
    // -------------------------------------------------------------------------
    // C++20 及以前：某些上下文（如返回类型是 && 或返回转换）先把表达式当 lvalue
    // 做重载决议，失败再当 xvalue（隐式移动）——规则难记且有坑。
    // C++23 P2266：move-eligible 的 id-expression 在 return/co_return/throw
    // 中直接当 xvalue，不再两阶段。

#if defined(__cpp_implicit_move)
    std::cout << "[advanced] __cpp_implicit_move=" << __cpp_implicit_move << '\n';
#else
    std::cout << "[advanced] __cpp_implicit_move not defined; toolchain may still "
                 "implement parts of P2266 under C++23 mode\n";
#endif

    reset_counts();
    [[maybe_unused]] Tracer b = make_for_throw();
    assert(b.id == 3);
    assert(Tracer::copies == 0);
    std::cout << "[advanced] another by-value return: moves=" << Tracer::moves << '\n';

    // -------------------------------------------------------------------------
    // 专家：与 std::move、悬空、向量实战
    // -------------------------------------------------------------------------
    // 1) 按值返回局部：写 return local; 即可，不必 return std::move(local);
    //    多余的 move 反而可能抑制 NRVO。
    // 2) 返回局部的 && / & 仍然是错误的（悬空），P2266 不把它变成安全。
    // 3) 隐式移动只针对自动存储期的可移动实体等 move-eligible 条件。

    [[maybe_unused]] Tracer&& rr = returns_rref_to_local_support();
    assert(rr.id == 2);

    std::vector<std::string> v;
    auto make_vec = []() {
        std::vector<std::string> local{"a", "b", "c"};
        return local;  // 隐式移动整个 vector
    };
    v = make_vec();
    assert(v.size() == 3 && v[1] == "b");

    std::cout << "[expert] prefer return local; avoid return std::move(local) for NRVO\n";
    std::cout << "=== simplified_implicit_move_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/simplified_implicit_move_cpp23", run>;

}  // namespace
