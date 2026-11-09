#include "stopcut.hpp"

namespace stopcut {

Play play(const std::string &input, const Options &options) {
    Play result{};
    std::string held;
    const std::vector<std::string> lines = split_lines(input);

    for (std::size_t index = 0; index < lines.size(); ++index) {
        const std::string chunk = read_chunk(lines[index], index + 1);
        result.bytes += chunk.size();
        const std::string buffer = held + chunk;

        std::size_t match_position = std::string::npos;
        std::size_t match_length = 0;
        for (const std::string &stop : options.stops) {
            const std::size_t position = buffer.find(stop);
            if (position != std::string::npos &&
                (match_position == std::string::npos || position < match_position ||
                 (position == match_position && stop.size() > match_length))) {
                match_position = position;
                match_length = stop.size();
            }
        }

        if (match_position != std::string::npos) {
            const std::size_t end = match_position + (options.keep ? match_length : 0);
            std::string emit = buffer.substr(0, end);
            result.total += emit.size();
            result.steps.push_back(Step{emit, "", true});
            result.stopped = true;
            return result;
        }

        std::size_t hold_length = 0;
        for (const std::string &stop : options.stops) {
            const std::size_t limit = stop.size() - 1;
            const std::size_t maximum = buffer.size() < limit ? buffer.size() : limit;
            for (std::size_t length = maximum; length > hold_length; --length) {
                if (buffer.compare(buffer.size() - length, length, stop, 0, length) == 0) {
                    hold_length = length;
                    break;
                }
            }
        }

        const std::size_t emit_length = buffer.size() - hold_length;
        std::string emit = buffer.substr(0, emit_length);
        held = buffer.substr(emit_length);
        result.total += emit.size();
        if (held.size() > result.peak) {
            result.peak = held.size();
        }
        result.steps.push_back(Step{emit, held, false});
    }

    result.flush = held;
    result.total += result.flush.size();
    return result;
}

} // namespace stopcut
