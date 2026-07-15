// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : main_and_program_structure
// Topic id : part2/stage01/section01/main_and_program_structure
//
// Covers: hosted entry point, argc/argv, EXIT_SUCCESS; run() simulates main (no real main here)

#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <cstring>
#include <string_view>

namespace {

// Simulated hosted main signature — the real process main lives in learn_cpp's driver.
int simulated_main(int argc, char** argv) {
    LEARN_CHECK(argc >= 1);
    LEARN_CHECK(argv != nullptr);
    LEARN_CHECK(argv[argc] == nullptr);

    if (argc >= 1 && argv[0] != nullptr) {
        LEARN_CHECK(std::strlen(argv[0]) > 0 || argv[0][0] == '\0');
    }
    return EXIT_SUCCESS;
}

void demo_basics() {
    char arg0[] = "learn_cpp";
    char* argv[] = {arg0, nullptr};
    const int argc = 1;

    LEARN_CHECK(simulated_main(argc, argv) == EXIT_SUCCESS);
    LEARN_CHECK(EXIT_SUCCESS == 0);
}

void demo_intermediate() {
    char arg0[] = "prog";
    char arg1[] = "--verbose";
    char arg2[] = "42";
    char* argv[] = {arg0, arg1, arg2, nullptr};
    const int argc = 3;

    LEARN_CHECK(argc >= 1);
    LEARN_CHECK(argv[argc] == nullptr);
    LEARN_CHECK(std::string_view{argv[0]} == "prog");
    LEARN_CHECK(std::string_view{argv[1]} == "--verbose");
    LEARN_CHECK(std::string_view{argv[2]} == "42");

    int count = 0;
    for (int i = 0; i < argc; ++i) {
        LEARN_CHECK(argv[i] != nullptr);
        ++count;
    }
    LEARN_CHECK(count == argc);
    LEARN_CHECK(simulated_main(argc, argv) == EXIT_SUCCESS);
}

void demo_expert() {
    // Hosted implementation: static-duration objects initialize before main;
    // returning from main is equivalent to std::exit(return_value) after
    // destroying automatic objects in main. We only model the call interface.
    static int static_init_marker = 0;
    ++static_init_marker;
    LEARN_CHECK(static_init_marker >= 1);

    char arg0[] = "tool";
    char* empty_extra[] = {arg0, nullptr};
    LEARN_CHECK(simulated_main(1, empty_extra) == EXIT_SUCCESS);

    // argv strings are mutable in the abstract machine; mutation is allowed.
    char mutable_name[] = "app";
    char* mut_argv[] = {mutable_name, nullptr};
    mut_argv[0][0] = 'A';
    LEARN_CHECK(std::string_view{mut_argv[0]} == "App");
    LEARN_CHECK(simulated_main(1, mut_argv) == EXIT_SUCCESS);
}

int run(int argc, char** argv) {
    // When invoked by the driver, argc/argv are the real process arguments.
    if (argc >= 1 && argv != nullptr && argv[argc] == nullptr) {
        LEARN_CHECK(simulated_main(argc, argv) == EXIT_SUCCESS);
    }

    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/main_and_program_structure", run>;

}  // namespace
