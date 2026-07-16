#include "learn/topic_registry.hpp"

#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    if (argc < 2) {
#ifdef NDEBUG
        // Release/RelWithDebInfo: list topics so the user can pick one.
        learn::list_topics();
        return 0;
#else
        // Debug: hand off to run_topic with no id; it iterates the whole map.
        // F5 from the IDE lands inside whichever topic has a breakpoint.
        return learn::run_topic(argc - 1, argv + 1);
#endif
    }

    const std::string_view first{argv[1]};
    if (first == "--help" || first == "-h" || first == "help") {
        std::cout << "usage: learn_cpp [topic_id [extra_args...]]\n"
                  << "  (no args, release)  list every registered topic\n"
                  << "  (no args, debug)    iterate every topic; useful for F5-from-IDE\n"
                  << "  --list              list every registered topic without running one\n"
                  << "  topic_id            run that topic; extra args reach its run()\n"
                  << "  --help/-h           show this message\n";
        return 0;
    }
    if (first == "--list") {
        learn::list_topics();
        return 0;
    }

    return learn::run_topic(argc - 1, argv + 1);
}
