// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : new_c_compat_headers
// Topic id : part2/stage14/section08/new_c_compat_headers
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<stdatomic.h>)
#include <stdatomic.h>
#endif
#include <atomic>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section08/new_c_compat_headers";

constexpr bool has_stdatomic_header =
#if __has_include(<stdatomic.h>)
    true;
#else
    false;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::atomic<int> value{1};
    value.fetch_add(2, std::memory_order_relaxed);
    LEARN_EXPECT_EQ(checks, value.load(std::memory_order_relaxed), 3);
#if defined(__cpp_lib_stdatomic_h) && __cpp_lib_stdatomic_h >= 202011L
    LEARN_EXPECT(checks, has_stdatomic_header);
#else
    LEARN_EXPECT(checks, has_stdatomic_header || !has_stdatomic_header);
#endif
    // C++23's <stdatomic.h> supports source headers shared with C; ordinary C++ code still prefers <atomic>.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage14/section08/new_c_compat_headers", run>;

}  // namespace
