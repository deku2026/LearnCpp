// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section01_features_by_standard_cpp11
// Item     : cpp11_language_features_overview
// Topic id : part2/stage14/section01/cpp11_language_features_overview
//
// C++11 language feature micro-matrix: one runnable demo cell per signature item.
// Macro anchors (predefined since C++20 feature-test): __cpp_decltype, __cpp_lambdas,
// __cpp_rvalue_references, __cpp_variadic_templates, __cpp_constexpr, __cpp_static_assert, ...

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// --- auto / decltype / trailing return ---
template <class T, class U>
auto add_trailing(T a, U b) -> decltype(a + b) {
    return a + b;
}

// --- rvalue refs / move / perfect forward ---
struct Box {
    int v = 0;
    Box() = default;
    explicit Box(int x) : v(x) {}
    Box(const Box& o) : v(o.v) { ++copies; }
    Box(Box&& o) noexcept : v(o.v) {
        o.v = -1;
        ++moves;
    }
    Box& operator=(const Box&) = default;
    Box& operator=(Box&&) noexcept = default;
    static int copies;
    static int moves;
};
int Box::copies = 0;
int Box::moves = 0;

template <class T>
T&& fwd_id(T&& x) {
    return std::forward<T>(x);
}

// --- variadic templates ---
int sum_pack() {
    return 0;
}
template <class T, class... Ts>
int sum_pack(T head, Ts... tail) {
    return static_cast<int>(head) + sum_pack(tail...);
}

// --- user-defined literal ---
constexpr long double operator""_km(long double v) {
    return v * 1000.0L;
}

// --- override / final ---
struct Base {
    virtual ~Base() = default;
    virtual int f() const { return 1; }
};
struct Derived final : Base {
    int f() const override { return 2; }
};

// --- enum class / nullptr / alignas ---
enum class Color : std::uint8_t { Red = 1, Green = 2, Blue = 3 };

struct alignas(16) Aligned16 {
    char data[16];
};

// --- delegated / inherited ctor / NSDMI ---
struct Point {
    int x = 0;  // NSDMI
    int y = 0;
    Point() = default;
    Point(int a, int b) : x(a), y(b) {}
    Point(int a) : Point(a, 0) {}  // delegated
};
struct NamedPoint : Point {
    using Point::Point;  // inherited constructors
    std::string name;
};

// --- ref-qualified members ---
struct RQ {
    int v = 7;
    int& get() & { return v; }
    int get() && { return v; }
};

// --- lambda ---
int run_lambda_cell() {
    int capture = 10;
    auto add = [capture](int x) { return capture + x; };
    auto mut = [n = 0]() mutable { return ++n; };  // init capture is C++14; plain C++11:
    (void)mut;
    auto pure = [](int a, int b) { return a * b; };
    return add(5) + pure(3, 4);  // 15 + 12 = 27
}

// --- constexpr / static_assert ---
constexpr int square(int n) {
    return n * n;
}
static_assert(square(4) == 16, "C++11 constexpr");
static_assert(alignof(Aligned16) >= 16, "alignas/alignof");

// --- attributes / noexcept ---
void maybe_throw(bool b) noexcept(!true) {
    if (b) {
        throw 1;
    }
}

// inline namespace at namespace scope (C++11)
inline namespace inline_ns_demo {
inline int api() {
    return 11;
}
}  // namespace inline_ns_demo
inline int inline_ns_demo_api() {
    return api();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++11 language features micro-matrix ===\n";

    // 1) auto / decltype / trailing return
    auto i = 42;
    decltype(i) j = i + 1;
    auto sum = add_trailing(1.5, 2);
    std::cout << "[auto/decltype/trailing] i=" << i << " j=" << j << " sum=" << sum << '\n';
    assert(j == 43);

    // 2) range-for + initializer_list + uniform init
    std::vector<int> xs{1, 2, 3, 4};
    int acc = 0;
    for (int x : xs) {
        acc += x;
    }
    std::initializer_list<int> il = {10, 20, 30};
    for (int v : il) {
        acc += v;
    }
    std::cout << "[range-for/init-list/brace-init] acc=" << acc << '\n';
    assert(acc == 70);

    // 3) nullptr / enum class
    int* p = nullptr;
    Color c = Color::Green;
    std::cout << "[nullptr/enum class] p_is_null=" << (p == nullptr) << " color=" << static_cast<int>(c) << '\n';

    // 4) rvalue refs / move / perfect forward
    Box::copies = Box::moves = 0;
    Box a{5};
    Box b = std::move(a);
    Box c2 = fwd_id(Box{9});
    std::cout << "[move/forward] b.v=" << b.v << " c2.v=" << c2.v << " copies=" << Box::copies
              << " moves=" << Box::moves << '\n';
    assert(b.v == 5);

    // 5) variadic templates
    const int pack_sum = sum_pack(1, 2, 3, 4);
    std::cout << "[variadic templates] sum_pack=" << pack_sum << '\n';
    assert(pack_sum == 10);

    // 6) lambda
    const int lam = run_lambda_cell();
    std::cout << "[lambda] result=" << lam << '\n';
    assert(lam == 27);

    // 7) constexpr / static_assert (assert already at compile-time above)
    constexpr int k = square(5);
    std::cout << "[constexpr] square(5)=" << k << '\n';

    // 8) using alias / alias template
    using IntVec = std::vector<int>;
    IntVec v{7, 8};
    std::cout << "[using alias] v.size=" << v.size() << '\n';

    // 9) delegated / inherited ctor / NSDMI / defaulted-deleted
    Point p1{3};
    NamedPoint np{4, 5};
    struct NoCopy {
        NoCopy() = default;
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator=(const NoCopy&) = delete;
    };
    NoCopy nc;
    (void)nc;
    std::cout << "[ctor suite] p1=(" << p1.x << ',' << p1.y << ") np=(" << np.x << ',' << np.y << ")\n";

    // 10) override / final / ref-qualifiers
    Derived d;
    Base& br = d;
    RQ rq;
    int& lref = rq.get();
    int rref_val = RQ{}.get();
    std::cout << "[override/final/ref-qual] virtual=" << br.f() << " lref=" << lref << " rvalue_get=" << rref_val
              << '\n';
    assert(br.f() == 2);

    // 11) UDL / raw string / unicode char types / attributes
    const long double meters = 1.5_km;
    const char* raw = R"(line1\nnot-escaped)";
    char16_t u16 = u'A';
    char32_t u32 = U'B';
    std::cout << "[UDL/raw/u16/u32] meters=" << static_cast<double>(meters)
              << " raw_len=" << std::char_traits<char>::length(raw) << " u16=" << static_cast<int>(u16)
              << " u32=" << static_cast<int>(u32) << '\n';

    // 12) alignas / alignof / thread_local / noexcept / >> fix
    thread_local int tls = 1;
    ++tls;
    Aligned16 a16{};
    std::vector<std::vector<int>> nested{{1}, {2, 3}};  // >> closing fix
    maybe_throw(false);                                 // ODR-use for noexcept demo companion
    std::cout << "[align/tls/noexcept/>>] alignof=" << alignof(Aligned16) << " tls=" << tls
              << " nested_rows=" << nested.size() << " noexcept(square)=" << noexcept(square(1))
              << " noexcept(maybe_throw)=" << noexcept(maybe_throw(false)) << '\n';
    (void)a16;

    // 13) explicit conversion operator
    struct Boolish {
        explicit operator bool() const { return true; }
    };
    if (Boolish{}) {
        std::cout << "[explicit conversion] Boolish is true in boolean context\n";
    }

    // 14) inline namespace (defined at namespace scope below)
    std::cout << "[inline namespace] api()=" << inline_ns_demo::api() << " = " << inline_ns_demo_api() << '\n';

    std::cout << "C++11 language matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section01/cpp11_language_features_overview", run>;

}  // namespace
