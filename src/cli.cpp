#include "stopcut.hpp"

namespace stopcut {

static const Command *find_command(const std::string &name) {
    for (const Command &command : COMMANDS) {
        if (name == command.name) {
            return &command;
        }
    }
    return nullptr;
}

static Options parse_options(const Command &command, const std::vector<std::string> &args) {
    Options options{{}, false};
    for (std::size_t index = 1; index < args.size(); ++index) {
        const std::string &word = args[index];
        if (word.empty() || word == "-" || word[0] != '-') {
            throw Failure{2, "unexpected argument " + word};
        }
        if (word == "--keep") {
            if (!command.allows_keep) {
                throw Failure{2, "unknown option " + word};
            }
            if (options.keep) {
                throw Failure{2, "given twice: --keep"};
            }
            options.keep = true;
            continue;
        }
        if (word != "--stop") {
            throw Failure{2, "unknown option " + word};
        }
        if (index + 1 >= args.size()) {
            throw Failure{2, "missing value for --stop"};
        }
        const std::string &value = args[++index];
        bool ok = false;
        std::string stop = unescape(value, ok);
        if (!ok) {
            throw Failure{2, "bad escape in stop sequence"};
        }
        if (stop.empty()) {
            throw Failure{2, "empty stop sequence"};
        }
        if (stop.size() > 16) {
            throw Failure{2, "stop sequence is too long (max 16 bytes)"};
        }
        for (const std::string &prior : options.stops) {
            if (stop == prior) {
                throw Failure{2, "duplicate stop sequence"};
            }
        }
        if (options.stops.size() >= 8) {
            throw Failure{2, "too many stop sequences (max 8)"};
        }
        options.stops.push_back(stop);
    }
    if (command.needs_stop && options.stops.empty()) {
        throw Failure{2, "missing --stop"};
    }
    return options;
}

static const Command *validated_command(const std::vector<std::string> &args, Options &options) {
    if (args.empty()) {
        return nullptr;
    }
    const Command *command = find_command(args[0]);
    if (command == nullptr) {
        return nullptr;
    }
    options = parse_options(*command, args);
    return command;
}

bool reads_stdin(const std::vector<std::string> &args) {
    if (args.empty() || args[0] == "help" || args[0] == "-h" || args[0] == "--help" || args[0] == "--version") {
        return false;
    }
    try {
        Options options{{}, false};
        return validated_command(args, options) != nullptr;
    } catch (const Failure &) {
        return false;
    }
}

Result run(const std::vector<std::string> &args, const std::string &input) {
    if (args.empty()) {
        return Result{2, "", USAGE};
    }
    if (args[0] == "help" || args[0] == "-h" || args[0] == "--help") {
        return Result{0, USAGE, ""};
    }
    if (args[0] == "--version") {
        return Result{0, "stopcut 0.1.0\n", ""};
    }

    const Command *command = find_command(args[0]);
    if (command == nullptr) {
        return Result{2, "", "stopcut: unknown command " + args[0] + "\n"};
    }
    try {
        const Options options = parse_options(*command, args);
        return command->run(options, input);
    } catch (const Failure &failure) {
        return Result{failure.status, "", "stopcut: " + failure.message + "\n"};
    }
}

} // namespace stopcut
