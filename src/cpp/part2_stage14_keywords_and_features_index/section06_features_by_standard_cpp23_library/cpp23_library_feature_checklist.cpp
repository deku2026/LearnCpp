// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section06_features_by_standard_cpp23_library
// Item     : cpp23_library_feature_checklist
// Topic id : part2/stage14/section06/cpp23_library_feature_checklist
//
// C++23 library signature checklist. Every cell gated by __cpp_lib_* /
// __has_include so the TU still builds on partial library implementations.

#include "learn/topic_registry.hpp"

#if defined(__has_include)
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARNCPP_HAS_MDSPAN 1
#endif
#endif
#ifndef LEARNCPP_HAS_MDSPAN
#define LEARNCPP_HAS_MDSPAN 0
#endif

#if !LEARNCPP_HAS_MDSPAN
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] <mdspan> not available on this standard library\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section06/cpp23_library_feature_checklist", run>;
}  // namespace
#else

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#endif
#if __has_include(<mdspan>) && defined(__cpp_lib_mdspan)
#endif
#if __has_include(<generator>) && defined(__cpp_lib_generator)
#include <generator>
#endif
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <type_traits>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#endif
#if __has_include(<stacktrace>) && defined(__cpp_lib_stacktrace)
#include <stacktrace>
#endif
#if __has_include(<spanstream>) && defined(__cpp_lib_spanstream)
#include <spanstream>
#endif
#include <bit>
#include <utility>

namespace {

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> gen_upto(int n) {
    for (int i = 0; i < n; ++i) {
        co_yield i;
    }
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++23 library feature checklist ===\n";

    // 1) expected
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    {
        std::expected<int, std::string> ok = 42;
        std::expected<int, std::string> err = std::unexpected(std::string{"boom"});
        auto mapped = ok.transform([](int x) { return x + 1; });
        std::cout << "[expected] *ok=" << *ok << " err=" << err.error() << " mapped=" << *mapped
                  << " macro=" << __cpp_lib_expected << '\n';
        assert(ok && *mapped == 43 && !err);
    }
#else
    std::cout << "[expected] SKIP\n";
#endif

    // 2) print / println
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    {
        std::print("[print] answer={}\n", 42);
        std::println("[println] hi {}", "C++23");
        std::cout << "  macro=" << __cpp_lib_print << '\n';
    }
#else
    std::cout << "[print] SKIP\n";
#endif

    // 3) mdspan
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    {
        std::array<int, 6> buf{1, 2, 3, 4, 5, 6};
        std::mdspan<int, std::extents<std::size_t, 2, 3>> m(buf.data());
        std::cout << "[mdspan] m[1,2]=" << m[1, 2] << " macro=" << __cpp_lib_mdspan << '\n';
        assert((m[1, 2] == 6));
    }
#else
    std::cout << "[mdspan] SKIP\n";
#endif

    // 4) generator
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    {
        int sum = 0;
        for (int x : gen_upto(4)) {
            sum += x;
        }
        std::cout << "[generator] sum0..3=" << sum << " macro=" << __cpp_lib_generator << '\n';
        assert(sum == 6);
    }
#else
    std::cout << "[generator] SKIP\n";
#endif

    // 5) ranges::to + new views (zip/enumerate if present)
#if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
    {
        std::vector<int> data{1, 2, 3, 4, 5};
        auto evens = data | std::views::filter([](int x) { return x % 2 == 0; }) | std::ranges::to<std::vector>();
        std::cout << "[ranges::to] size=" << evens.size() << " first=" << evens.front()
                  << " macro=" << __cpp_lib_ranges_to_container << '\n';
        assert(evens.size() == 2 && evens[0] == 2);
    }
#else
    std::cout << "[ranges::to] SKIP\n";
#endif

#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    {
        std::vector a{1, 2};
        std::vector b{10, 20};
        int sum = 0;
        for (auto [x, y] : std::views::zip(a, b)) {
            sum += x + y;
        }
        std::cout << "[views::zip] sum=" << sum << '\n';
        assert(sum == 33);
    }
#else
    std::cout << "[views::zip] SKIP\n";
#endif

    // 6) ranges fold
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    {
        std::vector v{1, 2, 3, 4};
        const auto left = std::ranges::fold_left(v, 0, std::plus<>{});
        std::cout << "[ranges fold] left=" << left << " macro=" << __cpp_lib_ranges_fold << '\n';
        assert(left == 10);
    }
#else
    std::cout << "[ranges fold] SKIP\n";
#endif

    // 7) flat_map
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    {
        std::flat_map<int, std::string> fm{{1, "a"}, {2, "b"}};
        std::cout << "[flat_map] fm[2]=" << fm[2] << " macro=" << __cpp_lib_flat_map << '\n';
        assert(fm[2] == "b");
    }
#else
    std::cout << "[flat_map] SKIP\n";
#endif

    // 8) optional monadic ops (C++23)
#if (defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L) || defined(__cpp_lib_monadic_optional)
    {
        std::optional<int> o = 21;
        auto r = o.transform([](int x) { return x * 2; });
        std::cout << "[optional monadic] " << *r << '\n';
        assert(r && *r == 42);
    }
#else
    std::cout << "[optional monadic] SKIP\n";
#endif

    // 9) move_only_function / bind_back / forward_like / invoke_r
#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
    {
        std::move_only_function<int(int)> f = [u = std::make_unique<int>(2)](int x) { return x * *u; };
        std::cout << "[move_only_function] " << f(21) << " macro=" << __cpp_lib_move_only_function << '\n';
        assert(f(21) == 42);
    }
#else
    std::cout << "[move_only_function] SKIP\n";
#endif

#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    {
        auto div = [](int a, int b) { return a / b; };
        auto half = std::bind_back(div, 2);
        std::cout << "[bind_back] " << half(84) << '\n';
        assert(half(84) == 42);
    }
#else
    std::cout << "[bind_back] SKIP\n";
#endif

#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    {
        int x = 1;
        auto&& r = std::forward_like<const int&>(x);
        static_assert(std::is_same_v<decltype(r), const int&>);
        std::cout << "[forward_like] ok macro=" << __cpp_lib_forward_like << '\n';
    }
#else
    std::cout << "[forward_like] SKIP\n";
#endif

    // 10) byteswap / to_underlying / unreachable (careful) / string contains
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    {
        const std::uint16_t x = 0x1234;
        const auto y = std::byteswap(x);
        std::cout << "[byteswap] " << std::hex << y << std::dec << '\n';
        assert(y == 0x3412);
    }
#else
    std::cout << "[byteswap] SKIP\n";
#endif

#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    {
        enum class E : int { A = 7 };
        std::cout << "[to_underlying] " << std::to_underlying(E::A) << '\n';
        assert(std::to_underlying(E::A) == 7);
    }
#else
    std::cout << "[to_underlying] SKIP\n";
#endif

    {
        std::string s = "hello C++23";
#if defined(__cpp_lib_string_contains) || true
        // string::contains is C++23; always try when building as C++23
        std::cout << "[string::contains] " << s.contains("C++") << '\n';
        assert(s.contains("C++"));
#endif
    }

    // 11) stacktrace / spanstream / allocate_at_least (report macros)
#if defined(__cpp_lib_stacktrace)
    std::cout << "[stacktrace] macro=" << __cpp_lib_stacktrace;
#if __cpp_lib_stacktrace >= 202011L && __has_include(<stacktrace>)
    std::cout << " current_frames≈" << std::stacktrace::current().size();
#endif
    std::cout << '\n';
#else
    std::cout << "[stacktrace] SKIP\n";
#endif

#if defined(__cpp_lib_spanstream)
    std::cout << "[spanstream] macro=" << __cpp_lib_spanstream << '\n';
#else
    std::cout << "[spanstream] SKIP\n";
#endif

#if defined(__cpp_lib_allocate_at_least)
    {
        std::allocator<int> alloc;
        auto r = alloc.allocate_at_least(4);
        std::cout << "[allocate_at_least] requested=4 count=" << r.count << " macro=" << __cpp_lib_allocate_at_least
                  << '\n';
        alloc.deallocate(r.ptr, r.count);
    }
#else
    std::cout << "[allocate_at_least] SKIP\n";
#endif

    // 12) out_ptr
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    {
        std::unique_ptr<int> up;
        auto adopt = [](int** pp) { *pp = new int(42); };
        adopt(std::out_ptr(up));
        std::cout << "[out_ptr] " << *up << '\n';
        assert(up && *up == 42);
    }
#else
    std::cout << "[out_ptr] SKIP\n";
#endif

    std::cout << "C++23 library checklist OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section06/cpp23_library_feature_checklist", run>;

}  // namespace
#endif  // LEARNCPP_HAS_MDSPAN
