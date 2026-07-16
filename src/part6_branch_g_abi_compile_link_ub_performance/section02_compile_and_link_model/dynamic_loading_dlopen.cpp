// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : dynamic_loading_dlopen
// Topic id : part6/g/section02/dynamic_loading_dlopen
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <cstdint>
#include <string_view>

extern "C" {
using LearnCppTransformFunction = int(int) noexcept;
}

using Transform = LearnCppTransformFunction*;

struct PluginApiV1 {
    std::uint32_t struct_size{};
    std::uint32_t abi_version{};
    Transform transform{};
};

extern "C" int learn_cpp_plugin_transform(int value) noexcept {
    return value * 3;
}

extern "C" const PluginApiV1* learn_cpp_query_plugin_v1() noexcept {
    static const PluginApiV1 api{sizeof(PluginApiV1), 1, &learn_cpp_plugin_transform};
    return &api;
}

namespace {

constexpr std::string_view kTopic = "part6/g/section02/dynamic_loading_dlopen";

[[nodiscard]] constexpr bool compatible(const PluginApiV1& api) noexcept {
    return api.struct_size >= sizeof(PluginApiV1) && api.abi_version == 1 && api.transform != nullptr;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // dlopen/dlsym and LoadLibrary/GetProcAddress are platform APIs. The
    // language-level part worth testing is the versioned C-compatible table
    // returned after a loader resolves its stable factory symbol.
    const PluginApiV1 api = *learn_cpp_query_plugin_v1();
    LEARN_EXPECT(checks, compatible(api));
    LEARN_EXPECT_EQ(checks, api.transform(14), 42);

    PluginApiV1 wrong_version = api;
    wrong_version.abi_version = 2;
    LEARN_EXPECT(checks, !compatible(wrong_version));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/dynamic_loading_dlopen", run>;

}  // namespace
