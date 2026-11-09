#include "stopcut.hpp"

namespace stopcut {

Result check_command(const Options &options, const std::string &input) {
    const std::vector<std::string> lines = split_lines(input);
    std::vector<std::size_t> starts;
    std::vector<std::size_t> ends;
    std::string bytes;

    starts.reserve(lines.size());
    ends.reserve(lines.size());
    for (std::size_t index = 0; index < lines.size(); ++index) {
        const std::string chunk = read_chunk(lines[index], index + 1);
        starts.push_back(bytes.size());
        bytes += chunk;
        ends.push_back(bytes.size());
    }

    std::string output;
    for (std::size_t index = 0; index < options.stops.size(); ++index) {
        const std::string &stop = options.stops[index];
        const std::size_t occurrence = bytes.find(stop);
        output += "stop " + std::to_string(index + 1) + " \"" + escape(stop) + "\": ";
        if (occurrence == std::string::npos) {
            output += "absent\n";
            continue;
        }

        bool inside = false;
        for (std::size_t chunk = 0; chunk < starts.size(); ++chunk) {
            if (starts[chunk] <= occurrence && occurrence < ends[chunk] && stop.size() <= ends[chunk] - occurrence) {
                inside = true;
                break;
            }
        }
        output += inside ? "inside " : "split ";
        output += std::to_string(occurrence) + "\n";
    }
    output += "chunks " + std::to_string(lines.size()) + " bytes " + std::to_string(bytes.size()) + "\n";

    return Result{0, output, ""};
}

} // namespace stopcut
