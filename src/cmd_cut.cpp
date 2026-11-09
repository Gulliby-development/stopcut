#include "stopcut.hpp"

namespace stopcut {

Result cut_command(const Options &options, const std::string &input) {
    const Play result = play(input, options);
    std::string output;
    for (const Step &step : result.steps) {
        output += step.emit;
    }
    output += result.flush;
    return Result{0, escape(output) + "\n", ""};
}

} // namespace stopcut
