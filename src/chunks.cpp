#include "stopcut.hpp"

namespace stopcut {

std::vector<std::string> split_lines(const std::string &input) {
    std::vector<std::string> lines;
    std::size_t start = 0;

    while (start < input.size()) {
        const std::size_t newline = input.find('\n', start);
        const std::size_t end = newline == std::string::npos ? input.size() : newline;
        std::string line = input.substr(start, end - start);
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);

        if (newline == std::string::npos) {
            break;
        }
        start = newline + 1;
    }

    return lines;
}

std::string read_chunk(const std::string &line, std::size_t number) {
    bool ok = false;
    std::string chunk = unescape(line, ok);
    if (!ok) {
        throw Failure{1, "line " + std::to_string(number) + ": bad escape"};
    }
    if (chunk.size() > 64) {
        throw Failure{1, "line " + std::to_string(number) + ": chunk is too long (max 64 bytes)"};
    }
    return chunk;
}

} // namespace stopcut
