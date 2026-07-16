// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : declspec_dllexport
// Topic id : part6/g/section01/declspec_dllexport
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section01/declspec_dllexport";

#define LEARN_STRINGIZE_DETAIL(tokens) #tokens
#define LEARN_STRINGIZE(tokens) LEARN_STRINGIZE_DETAIL(tokens)

#if defined(_WIN32)
#define LEARN_DLL_EXPORT __declspec(dllexport)
#define LEARN_DLL_IMPORT __declspec(dllimport)
#else
#define LEARN_DLL_EXPORT
#define LEARN_DLL_IMPORT
#endif

constexpr std::string_view export_spelling = LEARN_STRINGIZE(LEARN_DLL_EXPORT);
constexpr std::string_view import_spelling = LEARN_STRINGIZE(LEARN_DLL_IMPORT);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

#if defined(_WIN32)
    LEARN_EXPECT(checks, export_spelling.find("dllexport") != std::string_view::npos);
    LEARN_EXPECT(checks, import_spelling.find("dllimport") != std::string_view::npos);
#else
    LEARN_EXPECT(checks, export_spelling.empty());
    LEARN_EXPECT(checks, import_spelling.empty());
#endif

#if 0
    // A real library places this declaration in its public header and selects
    // EXPORT while building the DLL, IMPORT while consuming it.
    extern "C" LEARN_DLL_EXPORT int library_entry(int);
#endif
    return checks.result();
}

#undef LEARN_DLL_IMPORT
#undef LEARN_DLL_EXPORT
#undef LEARN_STRINGIZE
#undef LEARN_STRINGIZE_DETAIL

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/declspec_dllexport", run>;

}  // namespace
