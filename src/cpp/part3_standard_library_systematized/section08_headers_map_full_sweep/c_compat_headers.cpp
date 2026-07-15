// Topic    : headers map · C 兼容头
// Doc      : 第3部分-标准库系统化.md · headers map C-compat
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : c_compat_headers
// Topic id : part3/section08/c_compat_headers
// Headers  : <cstdint> <cstring> <cstdio> <cstdlib> <cinttypes>
//            C++23: <xxx.h> 取消弃用，但仍优先 <cxxx>

#include "learn/topic_registry.hpp"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [c_compat_headers] map probe ===\n";
    std::cout << "prefer <cxxx> forms in pure C++ (cstdio not stdio.h)\n";
    std::cout << "C++23: <xxx.h> undeprecated, but <cxxx> still preferred\n";
    std::cout << "C++23 adds <stdatomic.h> (no <cstdatomic>)\n";
    std::cout << "C++26 (NOT C++23): <stdbit.h> <stdckdint.h>\n";

    static_assert(sizeof(std::int32_t) == 4);
    static_assert(sizeof(std::uint64_t) == 8);
    std::cout << "int32_t max via macros path: PRId32 format ok\n";

    char buf[32]{};
    std::snprintf(buf, sizeof(buf), "%" PRId32, std::int32_t{42});
    assert(std::string(buf) == "42");

    char dest[8]{};
    std::memcpy(dest, "abc", 4);
    assert(std::strcmp(dest, "abc") == 0);
    assert(std::strlen(dest) == 3);

    void* p = std::malloc(16);
    assert(p != nullptr);
    std::free(p);

    const int absv = std::abs(-7);
    assert(absv == 7);
    std::cout << "cstring/cstdio/cstdlib smoke ok abs=" << absv << '\n';

    std::cout << "[c_compat_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/c_compat_headers", run>;

}  // namespace
