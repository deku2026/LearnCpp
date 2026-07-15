// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section07_keyword_table
// Item     : keyword_taxonomy_a_to_p
// Topic id : part2/stage14/section07/keyword_taxonomy_a_to_p
//
// Keyword + feature taxonomy A–P: one runnable micro-cell per category.
// Context-sensitive keywords (override/final/requires) appear in their niches.

#include "learn/topic_registry.hpp"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <typeinfo>
#include <utility>

namespace {

// --- D: inheritance ---
struct PolyBase {
    virtual ~PolyBase() = default;
    virtual int id() const { return 1; }
};
struct PolyChild final : PolyBase {
    int id() const override { return 2; }
};

// --- G: concepts ---
template <class T>
concept Integralish = std::integral<T>;

template <Integralish T>
T inc(T x)
    requires(sizeof(T) >= 1)
{
    return static_cast<T>(x + 1);
}

// --- F: lambda / functions ---
inline int square_fn(int x) {
    return x * x;
}

// --- C: class / deducing-this style fallback ---
struct Holder {
    int n = 0;
    explicit Holder(int v) : n(v) {}
    Holder& operator+=(int x) {
        n += x;
        return *this;
    }
};

// --- M: coroutine keywords only named (full demo in stage12); use co_* free ---
// Avoid heavy promise here — category M is covered by co_await name plate via macro.

// A. program structure: namespace at namespace scope
namespace taxonomy_demo {
inline namespace v1 {
constexpr int k = 14;
}
}  // namespace taxonomy_demo

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== Keyword taxonomy A–P micro-matrix ===\n";

    // A. program structure: namespace / preprocessor already applied
    std::cout << "[A structure] demo::k=" << taxonomy_demo::k << " __cplusplus=" << __cplusplus << '\n';

    // B. type system: auto, decltype, consteval-ish constexpr, enum class, nullptr
    auto x = 10;
    decltype(x) y = x;
    enum class Kind : std::uint8_t { A, B };
    Kind k = Kind::B;
    int* p = nullptr;
    const volatile int cvi = 1;
    (void)cvi;
    std::cout << "[B types] y=" << y << " kind=" << static_cast<int>(k) << " null=" << (p == nullptr) << '\n';

    // C. class/object: class/struct, access, explicit, operator
    Holder h{40};
    h += 2;
    struct Agg {
        int a;
        int b;
    };
    Agg ag{1, 2};
    std::cout << "[C class] h.n=" << h.n << " ag=" << ag.a << ',' << ag.b << '\n';
    assert(h.n == 42);

    // D. inheritance / polymorphism / RTTI
    PolyChild child;
    PolyBase& br = child;
    std::cout << "[D inherit] id=" << br.id() << " typeid=" << typeid(br).name()
              << " dyn=" << dynamic_cast<PolyChild*>(&br)->id() << '\n';

    // E. copy/move
    auto up1 = std::make_unique<int>(5);
    auto up2 = std::move(up1);
    assert(!up1 && up2 && *up2 == 5);
    std::cout << "[E move] *up2=" << *up2 << '\n';

    // F. function / lambda
    auto lam = [](int a, int b) { return a + b; };
    std::cout << "[F function] square=" << square_fn(5) << " lam=" << lam(20, 22) << '\n';

    // G. templates / concepts / requires
    std::cout << "[G templates] inc(41)=" << inc(41) << '\n';
    assert(inc(41) == 42);

    // H. compile-time
    constexpr auto ce = [](int n) constexpr { return n * n; };
    static_assert(ce(3) == 9);
    std::cout << "[H compile-time] ce(6)=" << ce(6) << " square_fn(runtime)=" << square_fn(4) << '\n';

    // I. control flow
    int acc = 0;
    for (int i = 0; i < 3; ++i) {
        if (i == 1) {
            continue;
        }
        acc += i;
    }
    switch (acc) {
        case 2:
            break;
        default:
            assert(false);
    }
    std::cout << "[I control] acc=" << acc << '\n';

    // J. exceptions / noexcept
    auto may = [](bool b) noexcept(false) {
        if (b) {
            throw 1;
        }
        return 0;
    };
    try {
        may(false);
        std::cout << "[J exceptions] noexcept(may)=" << noexcept(may(false)) << '\n';
    } catch (...) {
        assert(false);
    }

    // K. STL touch (string as stand-in for library world)
    std::string s = "stl";
    s += "-ok";
    std::cout << "[K STL] s=" << s << '\n';

    // L. concurrency keywords: thread_local
    thread_local int tls = 0;
    ++tls;
    std::cout << "[L concurrency] thread_local=" << tls << '\n';

    // M. coroutines keywords plate
#if defined(__cpp_impl_coroutine)
    std::cout << "[M coroutines] __cpp_impl_coroutine=" << __cpp_impl_coroutine
              << " keywords: co_await co_yield co_return\n";
#else
    std::cout << "[M coroutines] macros not reported; keywords co_await/co_yield/co_return\n";
#endif

    // N. modules keywords plate
#if defined(__cpp_modules)
    std::cout << "[N modules] __cpp_modules=" << __cpp_modules << " keywords: module import export\n";
#else
    std::cout << "[N modules] context keywords: module / import / export\n";
#endif

    // O. memory: new/delete/alignas/alignof
    struct alignas(16) Line {
        char b[16];
    };
    auto* raw = new Line{};
    std::cout << "[O memory] alignof(Line)=" << alignof(Line) << '\n';
    delete raw;

    // P. literals / attributes / alternative tokens
    int bits = 0b1010;
    [[maybe_unused]] int unused = 0;
    bool logic = (true and not false) or false;
    std::cout << "[P literals/attrs/alt-tokens] bits=" << bits << " logic=" << logic << '\n';
    assert(logic);

    // context-sensitive: override/final already used; requires used above
    int override = 1;  // legal identifier outside member-fn virt-spec position
    int final = 2;
    std::cout << "[context-sensitive] override_id=" << override << " final_id=" << final << " (discouraged names)\n";

    std::cout << "Keyword taxonomy A–P OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section07/keyword_taxonomy_a_to_p", run>;

}  // namespace
