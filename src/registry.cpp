#include "stopcut.hpp"

namespace stopcut {

const char *const USAGE =
    "usage: stopcut cut --stop SEQ [--stop SEQ]... [--keep]\n"
    "       stopcut trace --stop SEQ [--stop SEQ]... [--keep]\n"
    "       stopcut stats --stop SEQ [--stop SEQ]... [--keep]\n"
    "       stopcut check [--stop SEQ]...\n";

const std::vector<Command> COMMANDS = {
    {"cut", true, true, cut_command},
    {"trace", true, true, trace_command},
    {"stats", true, true, stats_command},
    {"check", false, false, check_command},
};

} // namespace stopcut
