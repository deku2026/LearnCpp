// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : start_lifetime_as_in_systems_code_cpp23
// Topic id : part6/g/section04/start_lifetime_as_in_systems_code_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <type_traits>
#include <version>

namespace {

struct Record {
    int id;
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/start_lifetime_as_in_systems_code_cpp23"};

    static_assert(std::is_trivially_copyable_v<Record>);
    const Record source{40, 2};
    alignas(Record) std::array<std::byte, sizeof(Record)> storage{};
    std::memcpy(storage.data(), &source, sizeof(source));

#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    const Record* record = std::start_lifetime_as<Record>(storage.data());
    LEARN_EXPECT_EQ(checks, record->id + record->value, 42);
#else
    Record record{};
    std::memcpy(&record, storage.data(), sizeof(record));
    LEARN_EXPECT_EQ(checks, record.id + record.value, 42);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/start_lifetime_as_in_systems_code_cpp23", run>;

}  // namespace
