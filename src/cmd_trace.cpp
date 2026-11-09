#include "stopcut.hpp"

namespace stopcut {

Result trace_command(const Options &options, const std::string &input) {
    const Play result = play(input, options);
    std::string output;
    for (std::size_t index = 0; index < result.steps.size(); ++index) {
        const Step &step = result.steps[index];
        output += std::to_string(index + 1) + " emit \"" + escape(step.emit) + "\"";
        if (step.stopped) {
            output += " stop\n";
        } else {
            output += " hold \"" + escape(step.hold) + "\"\n";
        }
    }
    if (result.stopped) {
        output += "stopped " + std::to_string(result.total) + "\n";
    } else {
        output += "flushed " + std::to_string(result.total) + " \"" + escape(result.flush) + "\"\n";
    }
    return Result{0, output, ""};
}

Result stats_command(const Options &options, const std::string &input) {
    const Play result = play(input, options);
    const std::string stopped = result.stopped ? "yes" : "no";
    const std::string output = "chunks=" + std::to_string(result.steps.size()) + " bytes=" +
                               std::to_string(result.bytes) + " emitted=" + std::to_string(result.total) +
                               " held=" + std::to_string(result.peak) + " stopped=" + stopped + "\n";
    return Result{0, output, ""};
}

} // namespace stopcut
