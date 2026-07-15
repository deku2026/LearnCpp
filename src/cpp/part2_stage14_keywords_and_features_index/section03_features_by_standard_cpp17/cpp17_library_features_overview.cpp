// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section03_features_by_standard_cpp17
// Item     : cpp17_library_features_overview
// Topic id : part2/stage14/section03/cpp17_library_features_overview
//
// C++17 library micro-matrix: optional/variant/any/string_view/filesystem,
// invoke/apply, byte, clamp, reduce, gcd/lcm, shared_mutex, scoped_lock,
// map node extract/merge, not_fn, sample, execution policies, visit.

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <any>
#include <charconv>
#include <cstddef>
#include <execution>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <optional>
#include <random>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++17 library features micro-matrix ===\n";

    // 1) optional / variant / any / string_view
    std::optional<int> o = 10;
    o = std::nullopt;
    o = 21;
    std::variant<int, std::string> var = std::string{"hi"};
    std::any a = 3.14;
    std::string_view sv = "view-me";
    std::cout << "[optional/variant/any/sv] *o=" << *o << " var=" << std::get<std::string>(var)
              << " any_double=" << std::any_cast<double>(a) << " sv=" << sv << '\n';
    assert(o.value() == 21 && sv.size() == 7);

    // 2) filesystem (path algebra only — no disk I/O required)
    namespace fs = std::filesystem;
    const fs::path p = fs::path{"dir"} / "learncpp_stage14_touch.tmp";
    std::cout << "[filesystem] filename=" << p.filename().string() << " generic=" << p.generic_string() << '\n';
    assert(p.filename() == "learncpp_stage14_touch.tmp");

    // 3) invoke / apply
    auto add = [](int x, int y) { return x + y; };
    const int inv = std::invoke(add, 20, 22);
    const int ap = std::apply(add, std::make_tuple(10, 32));
    std::cout << "[invoke/apply] " << inv << " / " << ap << '\n';
    assert(inv == 42 && ap == 42);

    // 4) byte / clamp / gcd / lcm / reduce
    std::byte b{0xFF};
    const int cl = std::clamp(15, 0, 10);
    const int g = std::gcd(24, 36);
    const int l = std::lcm(4, 6);
    std::vector<int> nums{1, 2, 3, 4};
    const int red = std::reduce(nums.begin(), nums.end());
    std::cout << "[byte/clamp/gcd/lcm/reduce] byte=" << std::to_integer<int>(b) << " clamp=" << cl << " gcd=" << g
              << " lcm=" << l << " reduce=" << red << '\n';
    assert(cl == 10 && g == 12 && l == 12 && red == 10);

    // 5) to_chars / from_chars
    char buf[16]{};
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), 12345);
    assert(ec == std::errc{});
    int parsed = 0;
    auto [ptr2, ec2] = std::from_chars(buf, ptr, parsed);
    (void)ptr2;
    assert(ec2 == std::errc{} && parsed == 12345);
    std::cout << "[to_chars/from_chars] " << parsed << '\n';

    // 6) shared_mutex / scoped_lock
    std::shared_mutex sm;
    std::mutex m1;
    std::mutex m2;
    {
        std::shared_lock lk(sm);
        std::scoped_lock multi(m1, m2);
        std::cout << "[shared_mutex/scoped_lock] multi-lock held\n";
    }

    // 7) map extract / merge
    std::map<int, std::string> a_map{{1, "one"}, {2, "two"}};
    std::map<int, std::string> b_map{{3, "three"}};
    auto node = a_map.extract(1);
    node.key() = 10;
    b_map.insert(std::move(node));
    b_map.merge(a_map);
    std::cout << "[extract/merge] b_map.size=" << b_map.size() << " a_map.size=" << a_map.size() << '\n';
    assert(b_map.size() == 3 && a_map.empty());

    // 8) not_fn / sample
    auto is_even = [](int x) { return x % 2 == 0; };
    [[maybe_unused]] auto is_odd = std::not_fn(is_even);
    assert(is_odd(3));
    std::vector<int> pool{1, 2, 3, 4, 5, 6};
    std::vector<int> picks(2);
    std::sample(pool.begin(), pool.end(), picks.begin(), 2, std::mt19937{1});
    std::cout << "[not_fn/sample] picks=" << picks[0] << ',' << picks[1] << '\n';

    // 9) parallel policy type presence (name only — keep runs deterministic)
    (void)std::execution::seq;
    std::cout << "[execution] seq policy available\n";

    // 10) variant visit
    std::visit(
        [](auto&& val) {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "[visit] int=" << val << '\n';
            } else {
                std::cout << "[visit] string=" << val << '\n';
            }
        },
        var);

    std::cout << "C++17 library matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section03/cpp17_library_features_overview", run>;

}  // namespace
