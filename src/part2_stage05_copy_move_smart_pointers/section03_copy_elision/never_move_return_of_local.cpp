// LearnCpp placeholder
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : never_move_return_of_local
// Topic id : part2/stage05/section03/never_move_return_of_local
//
// TODO: read cppreference, sketch a minimal example, check godbolt / C++ Insights,
//       then replace this empty run() body with real demo code.

#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

LEARN_TOPIC("part2/stage05/section03/never_move_return_of_local", run);

}  // namespace
