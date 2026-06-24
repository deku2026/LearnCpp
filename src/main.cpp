#include "learn/topic_registry.hpp"

#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    if (argc < 2) {
        learn::list_topics();
        return 0;
    }

    const std::string_view first{argv[1]};
    if (first == "--help" || first == "-h" || first == "help") {
        std::cout << "usage: learn_cpp [topic_id [extra_args...]]\n"
                  << "  (no args)   list every registered topic\n"
                  << "  topic_id    run that topic; extra args reach its run()\n"
                  << "  --help/-h   show this message\n";
        return 0;
    }

    return learn::run_topic(argc - 1, argv + 1);
}
