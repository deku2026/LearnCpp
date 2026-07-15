// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section01_features_by_standard_cpp11
// Item     : cpp11_language_features_overview
// Topic id : part2/stage14/section01/cpp11_language_features_overview
//
// Covers: C++11 language feature sampler

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace {

void demo_basics() {
    auto x = 42;
    assert(x == 42);
    std::nullptr_t np = nullptr;
    int* p = np;
    assert(p == nullptr);
    std::string s = R"(raw\n)";
    assert(s == "raw\\n");
}

void demo_intermediate() {
    enum class Color : int { Red = 1, Blue = 2 };
    assert(static_cast<int>(Color::Red) == 1);
    std::vector<int> v{1, 2, 3};
    assert(v.size() == 3);
    auto lam = [](int a, int b) { return a + b; };
    assert(lam(2, 3) == 5);
}

void demo_expert() {
    static_assert(alignof(int) >= 1, "align");
    alignas(16) int buf[4]{};
    assert(reinterpret_cast<std::uintptr_t>(buf) % 16 == 0 || true);
    int&& r = 1;
    assert(r == 1);
    using std::move;
    int a = 3;
    int b = move(a);
    assert(b == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section01/cpp11_language_features_overview", run>;

}  // namespace
