// Topic    : headers map · 容器 containers
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : containers_headers
// Topic id : part3/section08/containers_headers
// Headers  : vector/array/deque/list/forward_list map/set unordered_*
//            stack/queue flat_* span mdspan bitset

#include "learn/topic_registry.hpp"

#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <span>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(__has_include)
#if __has_include(<flat_map>)
#include <flat_map>
#define LEARN_HAS_FLAT_MAP 1
#endif
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARN_HAS_MDSPAN 1
#endif
#endif
#ifndef LEARN_HAS_FLAT_MAP
#define LEARN_HAS_FLAT_MAP 0
#endif
#ifndef LEARN_HAS_MDSPAN
#define LEARN_HAS_MDSPAN 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [containers_headers] map probe ===\n";
    std::cout << "sequence: vector array deque list forward_list\n";
    std::cout << "assoc: map set unordered_map/set\n";
    std::cout << "adapter: stack queue priority_queue\n";
    std::cout << "views: span mdspan(C++23) bitset flat_*(C++23)\n";

    std::vector<int> v{1, 2, 3};
    [[maybe_unused]] std::array<int, 3> a{4, 5, 6};
    std::deque<int> d{7};
    std::list<int> l{8};
    std::forward_list<int> fl{9};
    assert(v.size() == 3 && a[1] == 5 && d.front() == 7 && l.front() == 8 && fl.front() == 9);

    std::map<std::string, int> m{{"a", 1}};
    std::set<int> s{1, 2, 2};
    std::unordered_map<int, int> um{{1, 10}};
    assert(m["a"] == 1 && s.size() == 2 && um[1] == 10);

    std::stack<int> st;
    st.push(3);
    assert(st.top() == 3);
    std::queue<int> q;
    q.push(4);
    assert(q.front() == 4);

    int raw[] = {1, 2, 3, 4};
    std::span<int> sp{raw};
    assert(sp.size() == 4 && sp[3] == 4);

    std::bitset<8> bits{0b1010};
    assert(bits.count() == 2);
    std::cout << "bitset=" << bits << '\n';

#if LEARN_HAS_FLAT_MAP
    std::flat_map<int, int> fm{{1, 2}};
    assert(fm[1] == 2);
    std::cout << "flat_map present\n";
#else
    std::cout << "flat_map: not available\n";
#endif

#if LEARN_HAS_MDSPAN
    int data[2 * 3] = {1, 2, 3, 4, 5, 6};
    std::mdspan<int, std::extents<std::size_t, 2, 3>> md{data};
    assert(md.extent(0) == 2 && md.extent(1) == 3);
    // C++23 multi-dimensional operator[] (extra parens: assert is a macro)
    assert((md[0, 1] == 2));
    assert((md[1, 2] == 6));
    std::cout << "mdspan present extents=" << md.extent(0) << 'x' << md.extent(1) << " md[1,2]=" << md[1, 2] << '\n';
#else
    std::cout << "mdspan: not available\n";
#endif

    std::cout << "[containers_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/containers_headers", run>;

}  // namespace
