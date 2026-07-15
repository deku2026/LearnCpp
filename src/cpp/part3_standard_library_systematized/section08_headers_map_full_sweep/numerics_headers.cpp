// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : numerics_headers
// Topic id : part3/section08/numerics_headers
//
// Covers: headers map: cmath numbers numeric bit random complex valarray

#include "learn/topic_registry.hpp"

#include <bit>
#include <cmath>
#include <complex>
#include <numbers>
#include <numeric>
#include <random>
#include <vector>

namespace {

void demo_basics() {
    LEARN_CHECK(std::abs(-1) == 1);
    LEARN_CHECK(std::numbers::pi > 3.0);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(std::accumulate(v.begin(), v.end(), 0) == 6);
    LEARN_CHECK(std::popcount(0b101u) == 2);
}

void demo_expert() {
    std::complex<double> c{0, 1};
    LEARN_CHECK(c.imag() == 1.0);
    std::mt19937 eng{1};
    (void)eng();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/numerics_headers", run>;

}  // namespace
