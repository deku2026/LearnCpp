// Topic    : headers map · 内存 memory
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : memory_headers
// Topic id : part3/section08/memory_headers
// Headers  : <memory> <memory_resource> <new>

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <new>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [memory_headers] map probe ===\n";
    std::cout << "<memory>          unique/shared/weak, allocator, out_ptr, start_lifetime_as\n";
    std::cout << "<memory_resource> pmr polymorphic resources (C++17)\n";
    std::cout << "<new>             new_handler, align_val_t, launder, destroying_delete\n";

    auto up = std::make_unique<int>(1);
    auto sp = std::make_shared<int>(2);
    assert(*up == 1 && *sp == 2 && sp.use_count() == 1);
    std::cout << "unique/shared ok\n";

    std::array<std::byte, 512> buf{};
    std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};
    std::pmr::vector<int> v{&pool};
    v.push_back(42);
    assert(v.front() == 42);
    std::cout << "pmr::vector on monotonic buffer size=" << v.size() << '\n';

    std::new_handler handler = std::get_new_handler();
    std::cout << "get_new_handler ptr=" << reinterpret_cast<const void*>(handler) << '\n';

    void* raw = ::operator new(16, std::align_val_t{16});
    assert(raw != nullptr);
    ::operator delete(raw, std::align_val_t{16});
    std::cout << "aligned operator new/delete ok\n";

    std::cout << "[memory_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/memory_headers", run>;

}  // namespace
