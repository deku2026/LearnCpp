// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section02_features_by_standard_cpp14
// Item     : cpp14_library_features_overview
// Topic id : part2/stage14/section02/cpp14_library_features_overview
//
// C++14 library micro-matrix: make_unique, integer_sequence, exchange,
// shared_timed_mutex, transparent comparators / heterogeneous lookup.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <version>

namespace {

// integer_sequence application: expand tuple into vector via index pack
template <class Tuple, std::size_t... I>
std::vector<int> tuple_to_ints_impl(const Tuple& t, std::index_sequence<I...>) {
    return {static_cast<int>(std::get<I>(t))...};
}

template <class... Ts>
std::vector<int> tuple_to_ints(const std::tuple<Ts...>& t) {
    return tuple_to_ints_impl(t, std::index_sequence_for<Ts...>{});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++14 library features micro-matrix ===\n";

    // 1) make_unique
    auto p = std::make_unique<std::string>("unique");
    auto arr = std::make_unique<int[]>(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    std::cout << "[make_unique] *p=" << *p << " arr[2]=" << arr[2] << '\n';
    assert(*p == "unique" && arr[2] == 3);

    // 2) integer_sequence / index_sequence_for
    auto t = std::make_tuple(10, 20, 30);
    auto v = tuple_to_ints(t);
    std::cout << "[integer_sequence] v={";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << (i ? "," : "") << v[i];
    }
    std::cout << "}\n";
    assert(v.size() == 3 && v[0] == 10 && v[2] == 30);

    // 3) std::exchange
    int x = 5;
    int old = std::exchange(x, 9);
    std::cout << "[exchange] old=" << old << " x=" << x << '\n';
    assert(old == 5 && x == 9);

    // 4) shared_timed_mutex (shared lock + timed try)
    std::shared_timed_mutex stm;
    {
        std::shared_lock<std::shared_timed_mutex> rlock(stm);
        std::cout << "[shared_timed_mutex] shared_lock acquired\n";
    }
    {
        const bool got = stm.try_lock_for(std::chrono::milliseconds(1));
        std::cout << "[shared_timed_mutex] try_lock_for=" << got << '\n';
        if (got) {
            stm.unlock();
        }
    }

    // 5) transparent comparator + heterogeneous lookup (map with std::less<>)
    std::map<std::string, int, std::less<>> m{{"alpha", 1}, {"beta", 2}};
    // lookup with string_view-like: const char* without constructing std::string key
    auto it = m.find("beta");
    std::cout << "[transparent less<>] find(beta)=" << (it != m.end() ? it->second : -1) << '\n';
    assert(it != m.end() && it->second == 2);

    // 6) std::quoted (C++14 <iomanip>) — round-trip friendly string IO
    {
        std::ostringstream oss;
        const std::string raw = R"(he said "hi")";
        oss << std::quoted(raw);
        std::istringstream iss(oss.str());
        std::string back;
        iss >> std::quoted(back);
        assert(back == raw);
        std::cout << "[quoted] round-trip OK: " << oss.str() << '\n';
    }

    // 7) feature-test anchor (language already has generic lambdas etc.)
#if defined(__cpp_lib_make_unique)
    std::cout << "[feature-test] __cpp_lib_make_unique=" << __cpp_lib_make_unique << '\n';
#endif

    std::cout << "C++14 library matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section02/cpp14_library_features_overview", run>;

}  // namespace
